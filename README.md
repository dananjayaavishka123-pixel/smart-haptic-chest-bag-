# Smart Haptic Chest Bag | Wearable Assistive Navigation System

An ergonomic, torso-worn assistive technology designed to provide tactile spatial feedback for visually impaired individuals through high-frequency proximity tracking and custom PWM haptic arrays.

---

## 📌 Project Overview
Navigating dynamic indoor and outdoor environments poses significant spatial awareness challenges for visually impaired individuals. While traditional white canes detect ground-level obstacles, they frequently miss upper-torso and head-height hazards. 

The **Smart Haptic Chest Bag** bridges this gap by integrating an ultrasonic distance transducer array onto a wearable chest harness, translating real-world obstacle proximity into continuous, non-intrusive haptic vibration patterns.

---

## 🛠️ System Architecture & Features

- **Microcontroller:** ESP32 (Core Processing Engine)
- **Sensing:** Ultrasonic Distance Transducer Array
- **Feedback Mechanism:** Coreless Micro-Vibration Motors with dynamic PWM Duty-Cycle Scaling
- **Signal Filtering:** Software-level running average filter to eliminate environmental acoustic noise and signal jitter
- **Enclosure:** Custom textile pouch with rigid-backed internal chassis to ensure sensor alignment and component stabilization
[ Ultrasonic Transducer Array ]
│
▼ (Raw Signal + Acoustic Noise)
[ ESP32 Microcontroller ]
│
├─► [ Software Running-Average Filter ]
│
▼ (Calibrated PWM Signal Write)
[ Coreless Vibration Motors ] ──► [ Continuous Tactile Feedback ]

---

## 🛡️ Integrated Safety & Thermal Management System

To ensure user comfort and prevent thermal hazards during extended wear, the system incorporates automated active cooling, manual emergency signalling, and critical thermal shutdown protocols:

* **Automated Active Cooling (≥ 35°C):** When internal enclosure temperatures exceed **35°C**, the microcontroller automatically triggers the high-airflow cooling fans to maintain safe operating conditions.
* **Manual Emergency Help Signal:** Pressing the dedicated emergency push-button instantly activates:
  * Visual alert on the integrated **LCD display** ("Emergency Help").
  * High-intensity audible alerts via the **piezo buzzer**.
  * Immediate override to switch on all **cooling fans**.
* **Critical Thermal Auto-Shutdown (≥ 42°C):** If internal temperatures reach **42°C** (preventing skin discomfort or component degradation):
  * The system sounds an emergency buzzer pattern.
  * Displays a thermal alert on the LCD screen.
  * Executes a full system safety shutdown.

---

## 📊 Calibration & Testing Metrics

The system maps physical distance (D) directly to dynamic PWM duty cycles (Δ) across an active detection range of **50 cm to 10 cm**:

| Target Distance (D) | PWM Duty Cycle (Δ) | Haptic Feedback Profile |
| :--- | :--- | :--- |
| > 50 cm | 0% (Off) | Out of Range / Baseline Noise Floor |
| 50 cm | 25% | Initial Proximity Pulse |
| 30 cm | 60% | Moderate Dynamic Vibration |
| ≤ 10 cm | 100% (Max) | Continuous High-Intensity Alert |

---

## 🚀 Future Development Roadmap
- [ ] Transition from breadboard wiring to a custom **Rigid-Flex Printed Circuit Board (PCB)**.
- [ ] Integrate an **AI Camera Module (ESP32-S3)** for real-world computer vision object recognition.
- [ ] Implement **Kalman Filtering** for improved acoustic noise rejection.
- [ ] Achieve **IP65 Ingress Protection** rating for weather resistance.

---

## 👥 Authors & Acknowledgments
- **K. T. Dhananjaya Avishka Thanthilage** – Hardware & Software Integration
- **Faculty of Engineering, SLIIT** – ME1050 Introduction to Engineering Design & Communication

