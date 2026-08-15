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

