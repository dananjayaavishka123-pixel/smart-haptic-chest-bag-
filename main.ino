#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// =========================================================================
// 1. HARDWARE CONFIGURATIONS & INITIALIZATION
// =========================================================================

// Standard I2C display initialization
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// DHT11 Thermal Sensor 
const int pinDHT = 4;  
#define DHTTYPE DHT22         
DHT dht(pinDHT, DHTTYPE);

// Omron B3F Latching Switch (Input-Only Safe Pin 35)
const int pinButton = 35;  

// HC-SR04 Ultrasonic Distance Sensors
const int pinTriggerLeft   = 13;
const int pinEchoLeft      = 12; 
const int pinTriggerCenter = 14;
const int pinEchoCenter    = 27;
const int pinTriggerRight  = 26;
const int pinEchoRight     = 25;

// Output Drivers (Actuators via ULN2003 / MOSFETs)
const int pinMotorLeft     = 33; 
const int pinMotorCenter   = 32; 
const int pinMotorRight    = 19; 
const int pinFan           = 18; // Tied to IN4 on ULN2003
const int pinBuzzer        = 5;  

// =========================================================================
// 2. TIMERS & PARAMETERS
// =========================================================================
const int minDistance   = 10;   
const int maxDistance   = 50;   
const float tempFanOn   = 35.0; 
const float tempAlert   = 42.0; 

unsigned long lastBlinkTime = 0;
bool blinkState = true;
const int blinkInterval = 400; 

unsigned long lastDHTReadTime = 0;
const unsigned long dhtInterval = 2000; 
float currentTemp = 25.0;               

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("--- System Initializing ---");
  
  // Explicitly force the ESP32 I2C pins (SDA=21, SCL=22) to stabilize the bus
  Wire.begin(21, 22);
  delay(100);
  
  dht.begin();
  
  // Initialize the screen with safe timing delays
  lcd.init();
  lcd.backlight(); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Setup...");
  
  pinMode(pinEchoLeft, INPUT);
  pinMode(pinEchoCenter, INPUT);
  pinMode(pinEchoRight, INPUT);
  pinMode(pinButton, INPUT); 
  
  pinMode(pinTriggerLeft, OUTPUT);
  pinMode(pinTriggerCenter, OUTPUT);
  pinMode(pinTriggerRight, OUTPUT);
  pinMode(pinMotorLeft, OUTPUT);  
  pinMode(pinMotorCenter, OUTPUT);
  pinMode(pinMotorRight, OUTPUT);
  pinMode(pinFan, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  
  digitalWrite(pinMotorLeft, LOW);
  digitalWrite(pinMotorCenter, LOW);
  digitalWrite(pinMotorRight, LOW);
  digitalWrite(pinFan, LOW);
  digitalWrite(pinBuzzer, LOW);

  delay(1000); 
  lcd.clear();
  Serial.println("--- Initialization Complete! ---");
}

void loop() {
  int buttonState = digitalRead(pinButton);
  unsigned long currentMillis = millis();

  // =========================================================================
  // NON-BLOCKING TEMPERATURE SAMPLING
  // =========================================================================
  if (currentMillis - lastDHTReadTime >= dhtInterval) {
    lastDHTReadTime = currentMillis;
    float newTemp = dht.readTemperature();
    if (!isnan(newTemp) && newTemp > 5.0 && newTemp < 100.0) {
      currentTemp = newTemp; 
    }
  }

  bool emergencyActive = false;

  // =========================================================================
  // 3. AUTOMATION LOGIC & SAFETY GUARDRAILS
  // =========================================================================
  if (buttonState == HIGH || currentTemp >= tempAlert) {
    emergencyActive = true;
    digitalWrite(pinBuzzer, HIGH); 
    digitalWrite(pinFan, HIGH);    
    
    digitalWrite(pinMotorLeft, LOW);
    digitalWrite(pinMotorCenter, LOW);
    digitalWrite(pinMotorRight, LOW);
  } 
  else if (currentTemp >= tempFanOn) {
    digitalWrite(pinFan, HIGH);    
    digitalWrite(pinBuzzer, LOW);  
  } 
  else {
    digitalWrite(pinFan, LOW);     
    digitalWrite(pinBuzzer, LOW);  
  }

  // =========================================================================
  // 4. ULTRASONIC RANGEFINDER DRIVERS
  // =========================================================================
  long distanceLeft = 0, distanceCenter = 0, distanceRight = 0;

  if (!emergencyActive) {
    distanceLeft = getDistance(pinTriggerLeft, pinEchoLeft);
    digitalWrite(pinMotorLeft, (distanceLeft >= minDistance && distanceLeft <= maxDistance) ? HIGH : LOW);
    delay(10); 

    distanceCenter = getDistance(pinTriggerCenter, pinEchoCenter);
    digitalWrite(pinMotorCenter, (distanceCenter >= minDistance && distanceCenter <= maxDistance) ? HIGH : LOW);
    delay(10);

    distanceRight = getDistance(pinTriggerRight, pinEchoRight);
    digitalWrite(pinMotorRight, (distanceRight >= minDistance && distanceRight <= maxDistance) ? HIGH : LOW);
  }

  // =========================================================================
  // 5. SERIAL LOGGING
  // =========================================================================
  Serial.print("SW: "); Serial.print(buttonState);
  Serial.print(" | Temp: "); Serial.print((int)currentTemp);
  Serial.print("C | Distances -> L: "); Serial.print(distanceLeft);
  Serial.print("cm  C: "); Serial.print(distanceCenter);
  Serial.print("cm  R: "); Serial.print(distanceRight);
  Serial.println("cm");

  // =========================================================================
  // 6. I2C DISPLAY UI REFRESH
  // =========================================================================
  if (currentMillis - lastBlinkTime >= blinkInterval) {
    blinkState = !blinkState;
    lastBlinkTime = currentMillis;
  }

  if (emergencyActive) {
    if (buttonState == HIGH) {
      if (blinkState) {
        lcd.setCursor(0, 0); lcd.print("   EMERGENCY    ");
        lcd.setCursor(0, 1); lcd.print("      HELP      ");
      } else {
        lcd.setCursor(0, 0); lcd.print("                ");
        lcd.setCursor(0, 1); lcd.print("                ");
      }
    } else {
      lcd.setCursor(0, 0); lcd.print("!! OVERHEAT !!  ");
      lcd.setCursor(0, 1); lcd.print("REMOVE BELT NOW ");
    }
  } else {
    String tempDisplay = "TEMP: " + String((int)currentTemp) + (char)223 + "C    ";
    lcd.setCursor(0, 0);
    lcd.print(tempDisplay);
    
    lcd.setCursor(12, 0);
    if (currentTemp >= tempFanOn) lcd.print("[ON]");
    else lcd.print("[OK]");

    lcd.setCursor(0, 1);
    lcd.print("L"); lcd.print(distanceLeft); lcd.print("  ");
    lcd.setCursor(5, 1);
    lcd.print("C"); lcd.print(distanceCenter); lcd.print("  ");
    lcd.setCursor(11, 1);
    lcd.print("R"); lcd.print(distanceRight); lcd.print("  ");
  }

  delay(30); 
}

long getDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); 
  long calcDist = duration * 0.0343 / 2;
  
  if (calcDist <= 0 || calcDist > 400) {
    return 999; 
  }
  return calcDist;
}
