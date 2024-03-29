# Self-driving-Toy-Car

Basic toy car project for a young relative.  

Initially developed the firmware for an STM8S105 board. The code worked but the chip got destroyed due to some power issues.  
I decided to use a TI TM4C123 as a replacement. I developed the code using the TivaWare library and everything worked well 
until the USB port got damaged.

The STM8 and TM4C123 codes are in the Deprecated folder.  
The TM4C123 code was developed in Keil uVision 4 using an existing Tivaware project (called blinky) as a  
template. All application codes can be found in **main.c**.  

## How it works (Deprecated version)  
The car is controlled from an app using Bluetooth. Once it is given the command to start, it does the following:  
1. Moves forward  
2. Sweeps servo motor's shaft every 2 seconds  
3. Toggles an LED every 2 seconds  

## How it works (Current version)  
Once the app is used to start the car, the car moves autonomously and avoids obstacles.  
The app can also be used to stop the car at any moment in real-time.  
An ultrasonic sensor is mounted on the servo to look in different directions in order  
to avoid obstacles.  

## Components  
1. STM32 Blue Pill board  
2. L298N module  
3. Two 7805 regulators (one for HC05 and one for SG90 servo)  
4. SG90 servo motor  
5. 5mm LED + 2k resistor
6. Two TT gear motors with wheels   
7. On/Off switch  
8. Two 18650 Li-ion batteries  
9. HCSR04 ultrasonic sensor (current version)  
10. HC05 Bluetooth module  

## Power supply  
The 7.4v from the two Li-ion batteries is regulated to 5v using 7805 regulators. One of the   
regulators powers the STM32 Blue Pill, HCSR04, and HC05 module. The other regulator powers the SG90 servo. Both   
the servo and HC05 have separate power sources because the current consumption of the servo can cause  
the HC05 to keep resetting itself if both were powered by the same source.  The TT gear motors are routed  
directly to the 7.4v.  

The final prototype was developed using an STM32 Blue Pill board.  
The code was developed using STM32Cube HAL drivers in the STM32CubeIDE. The generic CMSIS  
RTOS API (v1) was also used to guarantee real-time operation.  

## The mobile application   
The Bluetooth app was made using MIT App Inventor.  

## Pinouts (Component - STM32)  
1. Servo - PA0    
2. LED - PB11   
3. Left motor pin 1 - PB14       
4. Left motor pin 2 - PA4     
5. Right motor pin 1 - PB15      
6. Right motor pin 2 - PA5    
7. Left motor PWM pin - PB0    
8. Right motor PWM pin - PB1    
9. Bluetooth - PA10    
10. Ultrasonic sensor Trig pin - PA6   
11. Ultrasonic sensor Echo pin - PB6    

## Prototype  
![20220812_085327](https://user-images.githubusercontent.com/46250887/192157650-8e8e15c8-9b4d-48ea-b3fd-8a9b424abe4f.jpg)



