# Wireless Master-Slave Control System (Engineering Thesis)

## Project Overview
This repository contains the source code and documentation for a wireless control system based on the ESP-NOW communication protocol. The project was developed as an engineering thesis to analyze the reliability and performance of low-latency wireless networks in embedded systems.

The system consists of a central **Master** unit and multiple **Slave** devices. It facilitates bidirectional communication, allowing the Master unit to control peripherals on Slave devices and receive real-time feedback. The project includes built-in diagnostic tools to measure network performance metrics such as Round Trip Time (RTT) and Packet Error Rate (PER).

## Technical Specifications
* **Hardware Platform:** ESP32 Microcontrollers
* **Communication Protocol:** ESP-NOW (Connectionless Wi-Fi protocol)
* **Programming Language:** C++ (Arduino Framework)
* **Display Interface:** SH1106 OLED (via I2C) utilizing the `U8g2lib` library
* **Network Topology:** Star topology (1 Master node, up to 8 Slave nodes)

## Key Features & Implementation Details

### 1. Communication Architecture
The system utilizes ESP-NOW for peer-to-peer communication, eliminating the need for a traditional Wi-Fi router. This ensures low latency and reduced power consumption.
* **Master Unit:** Manages the network, sends control commands ("Sprint" or "Wave" signals), and aggregates data from slaves.
* **Slave Units:** Listen for commands, actuate hardware (LEDs), and respond with acknowledgement packets.

### 2. Performance Analysis Tools
The firmware includes dedicated modules for quantitative analysis of the wireless link:
* **RTT (Round Trip Time) Measurement:** Calculates the time delay between sending a packet and receiving an acknowledgement in microseconds.
* **PER (Packet Error Rate) Analysis:** Conducts automated tests (e.g., sending 1000 packets) to determine the reliability of the link and the percentage of lost data.

### 3. Operational Modes
The system software implements distinct functional modes to demonstrate real-time control capabilities:
* **Sprint Mode:** A reaction-time test where the Master randomly activates a Slave unit, and the system measures the user's response time.
* **Wave Mode:** A sequential activation pattern demonstrating synchronized control across multiple nodes.

## Hardware Requirements
To replicate this setup, the following components are required:
* 2+ ESP32 Development Boards (1 Master, 1+ Slaves)
* 1x SH1106 OLED Display (connected to Master)
* Push buttons and LEDs (for Master control interface and Slave status indication)

## Setup and Installation
1.  **Dependencies:** Ensure the `U8g2` library is installed in your Arduino IDE or PlatformIO environment.
2.  **Mac Address Configuration:** Update the `slave_mac` arrays in the Master firmware with the actual MAC addresses of your ESP32 boards.
3.  **Flash Firmware:**
    * Upload `Master_Code` to the central unit.
    * Upload `Slave_Code` to each peripheral unit.

## Author
**Grzegorz Bach**
Contact: grzegorz.bach02@gmail.com
