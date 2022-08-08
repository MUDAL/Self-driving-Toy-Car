# Simple-Toy-Car

Basic toy car project for a kid.  

Initially developed firmware using an STM8S105 board (code worked) but the chip got fried due to some power issues.  
Decided to use a TI TM4C123 I had lying around. Developed the code using the TivaWare library and everything worked well 
until the USB port got damaged and for some reason, the board doesn't seem to work under external power.

The STM8 and TM4C123 codes are in the Deprecated folder as they won't be used for the final prototype.  
The TM4C123 code was developed in Keil uVision 4 using an existing Tivaware project (called blinky) as a  
template. All application codes can be found in **main.c**.  

## How it works  
The car is controlled from a app using Bluetooth. Once it is given the command to start, it does the following:  
1. Moves forward  
2. Sweeps servo motor's shaft every 2 seconds  
3. Toggles an LED every 2 seconds  

The final prototype was developed using an STM32 bluepill board.  
The code was developed using STM32Cube HAL drivers in the STM32CubeIDE.  
I didn't want to use the TM4C123 (initially) and the STM32 (eventually) for something so simple  
but in the end the code was developed for 3 different microcontrollers :) :) :)  

## The app  
The bluetooth app was made using MIT App Inventor.  


