# Smart Stick For The Blind

## Description
The project uses IoT to develop a smart cane for the blind, equipped with functions such as detecting obstacles, detecting water, turning on/off lights according to the light intensity of the environment, finding the cane remotely and send a warning email to relatives.

![Smart Stick](https://github.com/quocan0108/smart-stick-for-the-blind/blob/main/Image/stick-1.jfif)

![Smart Stick](https://github.com/quocan0108/smart-stick-for-the-blind/blob/main/Image/stick-2.jfif)

## Overview IoT Diagram
![Smart Stick](https://github.com/quocan0108/smart-stick-for-the-blind/blob/main/Image/IoT-diagram.png)
## Overview Hardware Diagram
![Smart Stick](https://github.com/quocan0108/smart-stick-for-the-blind/blob/main/Image/hardware-diagram.jfif)
## Hardware
- HC-SR04: Ultrasonic Sensor
- GY-521 6DOF IMU MPU6050: Acceleration Sensor
- LM393: Light Sensor
- V2: Rain Wate Sensor
- Mini Vibration Motor 3-5V
- HC-05: Module Bluetooth
- ESP8266 NodeMCU: Module Wifi
- LM2596: DC Voltage Reducer Circuit

## Libraries
- Arduino.h 
- ESP8266WiFi.h
- Wire.h
- math.h
- MPU6050.h
- EmailSender.h

## Contributors
- Le Quoc An
- Phan Chi Cuong
- Nguyen Quang Sang
