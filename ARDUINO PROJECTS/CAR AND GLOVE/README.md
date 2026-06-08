VID OF THE PROJECT ---> https://www.linkedin.com/posts/raz-tassa-3407722a6_electricalengineering-robotics-arduino-activity-7450573945692430336-PFEA?utm_source=share&utm_medium=member_desktop&rcm=ACoAAEnsLAUBKSBRPq7oXYJIkk6NgHjtJVP1fO4



# Gesture-Controlled Robotic Car

An open-source, wireless, gesture-controlled robotic car built using Arduino. Instead of traditional buttons or joysticks, this system utilizes a wearable transmitter unit that captures hand tilts via an IMU sensor and translates them into real-time driving commands. 

The chassis was custom-designed, modeled, and 3D-printed to perfectly house all onboard electronics.

---

## Features
* **Intuitive Gesture Control:** Uses an MPU6050 accelerometer/gyroscope to map hand angles to movement directions (Forward, Backward, Left, Right, Stop).
* **Robust Wireless Link:** 2.4 GHz communication using NRF24L01 modules.
* **Auto-Calibration:** Dynamic calibration phase during startup (`setup()`) to determine the hand's zero-point reference.
* **Smart Data Filtering:** Software logic optimizes bandwidth and avoids receiver freezing by updating motor states only upon command changes.
* **Hardware Noise Mitigation:** Power stabilization implementation to isolate electrical motor noise from the RF communication channel.
* **Custom 3D-Printed Chassis:** Specifically designed and fabricated for optimal weight distribution and component placement.

---

## Hardware Architecture

### Transmitter (Wearable Glove/Node)
* **Microcontroller:** Arduino Uno
* **Sensor:** MPU6050 (3-Axis Gyroscope & 3-Axis Accelerometer)
* **Wireless Module:** NRF24L01 (2.4 GHz RF Transceiver)

### Receiver (Robotic Car)
* **Microcontroller:** Arduino Uno
* **Wireless Module:** NRF24L01 (2.4 GHz RF Transceiver)
* **Motor Driver:** L298N Dual H-Bridge
* **Actuators:** 2x DC Motors
* **Power Supply:** 4x AA Battery Pack (for motor and logic supply)

---

## System Design & Engineering Challenges Solved

### 1. Data Flooding & Receiver Freezing (Software Logic)
Sending continuous, redundant data packets over the air can flood the receiver's buffer, causing lag or system freezing. To tackle this, we implemented a state-change detection logic in the code. Commands are only transmitted and applied to the motors when a distinct change in hand gesture state is detected, drastically lowering bandwidth consumption and ensuring smooth real-time response.

### 2. Electrical Noise & RF Interference (Hardware Mitigation)
During testing, the high-current draw and inductive switching from the DC motors generated significant electromagnetic interference (EMI) and voltage sags, which disrupted the NRF24L01 radio channel. We resolved this by introducing decoupling capacitors and power stabilization techniques to decouple the digital logic/RF supply from the noisy motor power rails.

### 3. Dynamic Zero-Point Calibration
To ensure the car doesn't drift due to variations in how the user wears the glove, an automated calibration sequence runs inside the `setup()` function. The system samples the initial resting position of the hand to establish an accurate center baseline before allowing navigation.

---

## Project Structure
```text
├── src/
│   ├── Transmitter/
│   │   └── Transmitter.ino      # Transmitter & MPU6050 sensing code
│   └── Receiver/
│       └── Receiver.ino         # Receiver, L298N logic, & noise filtering
├── hardware/
│   └── 3D_Models/               # STL files for the 3D-printed chassis
└── README.md
