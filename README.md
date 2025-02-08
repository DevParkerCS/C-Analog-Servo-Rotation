# C Analog Servo Control
## Description
This project demonstrates setting registers and bits by using bitwise operations, using an Analog-to-Digital Converter (ADC), and controlling a servo motor's rotation using Pulse Width Modulation (PWM)

## Hardware Requirements
Making this requires very minimal hardware to get working.  

- Arduino/Microcontroller (I used the Arduino UNO with the ATmega328p)
- Servo motor
- Potentiometer
- Breadboard
- Jumper wires
- External Power Supply

## Software Requirements
This project was built completely in C and was compiled and uploaded to the ATmega328p with the AVR toolchain.  

To download the AVR toolchain, please type the following commands below based on your OS.
### Linux
`sudo apt update`

`sudo apt install gcc-avr binutils-avr avr-libc avrdude`

###
