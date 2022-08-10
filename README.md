# Simple-Toy-Car

Basic toy car project for a young relative.  

Initially developed firmware using an STM8S105 board (code worked) but the chip got fried due to some power issues.  
Decided to use a TI TM4C123 I had lying around. Developed the code using the TivaWare library and everything worked well 
until the USB port got damaged and for some reason, the board doesn't seem to work under external power.

The STM8 and TM4C123 codes are in the Deprecated folder as they won't be used for the final prototype.  
The TM4C123 code was developed in Keil uVision 4 using an existing Tivaware project (called blinky) as a  
template. All application codes can be found in **main.c**.  

## How it works (Deprecated version)  
The car is controlled from an app using Bluetooth. Once it is given the command to start, it does the following:  
1. Moves forward  
2. Sweeps servo motor's shaft every 2 seconds  
3. Toggles an LED every 2 seconds  

## How it works (Current version)  
The car is still controlled by the app. But more features have been added.  
An ultrasonic sensor is mounted on the servo to look in different directions in order  
to avoid obstacles (although the field of view is limited).  

## Components  
1. STM32 Bluepill board  
2. L298N module  
3. Two 7805 regulators
4. SG90 servo motor  
5. 5mm LED + 2k resistor
6. Two TT gear motors  
7. On/Off switch  
8. Two 18650 Li-ion batteries  
9. HCSR04 ultrasonic sensor (current version)

The final prototype was developed using an STM32 bluepill board.  
The code was developed using STM32Cube HAL drivers in the STM32CubeIDE. The generic CMSIS  
RTOS API (v1) was also used to guarantee real-time operation.  

## The app  
The bluetooth app was made using MIT App Inventor.  


