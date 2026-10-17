# ESP32 Temperature-Controlled Fan

## Overview

An ESP32-based temperature-controlled fan system using a DHT11 temperature/humidity sensor and an L293D motor driver.

The DHT11 communication protocol was implemented from scratch in C++ using GPIO timing and bitwise operations, without external sensor libraries.

The system monitors ambient temperature every 10 seconds and automatically activates a DC fan when the temperature reaches 26°C.

## Features

- Custom DHT11 single-wire communication protocol
- 40-bit packet decoding using bitwise operations
- Microsecond-level pulse measurement using `micros()`
- 8-bit checksum validation and communication timeouts
- PWM-based fan ON/OFF control through an L293D motor driver
- Automatic fan shutdown on communication errors
- LED indicator and serial temperature/humidity monitoring

## Hardware

- ESP32 DevKit
- DHT11 temperature/humidity module
- L293D motor driver
- 3–6V DC fan
- Red LED with 230Ω resistor
- Breadboard and jumper wires

## Circuit Diagram

![ESP32 Fan Controller Circuit](hardware/esp32-fan-schematic.png)

## Project Photos

### Breadboard Setup

![ESP32 Breadboard Setup](images/ProjectPhoto_1.jpg)

### Additional View

![ESP32 Fan Controller](images/ProjectPhoto_2.jpg)

## How It Works

The ESP32 initiates communication with the DHT11 by pulling its data line LOW for 20 ms.

The sensor responds by transmitting a 40-bit packet containing temperature, humidity, and checksum data.

The firmware measures HIGH pulse durations to distinguish binary 0s and 1s, then reconstructs the received bytes using bitwise shifting.

After validating the checksum, the ESP32 compares the measured temperature against a 26°C threshold.

- **Temperature ≥ 26°C:** Fan and LED ON
- **Temperature < 26°C:** Fan and LED OFF
- **Communication failure:** Fan and LED OFF

The system repeats this process every 10 seconds.

## Software

- **Language:** C++
- **IDE:** Arduino IDE
- **Microcontroller:** ESP32
- **Sensor libraries:** None

**Source Code:** [temperature_controlled_fan.ino](firmware/temperature_controlled_fan/temperature_controlled_fan.ino)

## Challenges & Learning Outcomes

The main challenge was implementing the DHT11's timing-sensitive communication protocol without relying on a pre-existing library.

This involved interpreting the sensor datasheet, measuring microsecond-level GPIO pulses, reconstructing binary data, and debugging communication errors.

The project provided hands-on experience with embedded C++, hardware interfacing, bitwise operations, PWM, and defensive firmware design.

## Future Improvements

- Variable fan speed based on temperature
- Adjustable temperature thresholds
- Improved timing robustness against interrupts