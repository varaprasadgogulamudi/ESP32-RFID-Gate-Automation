# ESP32 RFID Gate Automation

An ESP32-based automated gate-control system using RFID authentication, a servo motor, and an ultrasonic sensor for vehicle passage detection.

## Project Overview

This project uses an ESP32 to control access to an automated gate. An RFID card is scanned by the MFRC522 RFID reader. If the scanned UID matches the authorized UID stored in the program, the servo motor opens the gate.

An ultrasonic sensor is used to detect vehicle passage. After the vehicle passes, the gate closes automatically.

## Components Used

- ESP32 Development Board
- MFRC522 RFID Reader
- RFID Card/Tag
- Servo Motor
- Ultrasonic Sensor
- Jumper Wires
- Breadboard / Prototype Setup
- External Power Supply

## Technologies Used

- ESP32
- Arduino IDE
- C/C++
- RFID
- MFRC522
- Servo Motor Control
- Ultrasonic Distance Measurement

## Pin Connections

| Component | ESP32 GPIO |
|---|---:|
| RFID SS | GPIO 5 |
| RFID RST | GPIO 22 |
| Ultrasonic TRIG | GPIO 13 |
| Ultrasonic ECHO | GPIO 12 |
| Servo Motor | GPIO 14 |

## Working Principle

1. The ESP32 initializes the RFID reader, ultrasonic sensor, and servo motor.
2. The system waits for an RFID card or tag.
3. The RFID UID is read and compared with the authorized UID.
4. If the UID is authorized, the servo rotates to open the gate.
5. The system waits for the vehicle to pass using the ultrasonic sensor.
6. After the vehicle passes, the servo returns to its initial position and closes the gate.
7. If the RFID UID is not authorized, access is denied.

## Project Features

- RFID-based access authentication
- Automatic gate opening using a servo motor
- Vehicle passage detection using an ultrasonic sensor
- Automatic gate closing
- Serial Monitor status messages
- ESP32-based embedded control

## Source Code

The Arduino source code is available in:

`ESP32_RFID_Gate_Automation.ino`

## Result

The prototype demonstrates automated gate access based on RFID authentication and automatic gate closing after vehicle passage detection.

## Future Scope

- Add a buzzer and LED status indication
- Add multiple authorized RFID cards
- Add a web/mobile monitoring interface
- Store access logs
- Add IoT-based remote monitoring
