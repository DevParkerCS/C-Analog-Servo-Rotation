# C Analog Servo Control
![Servo Example](/assets/example.mov)

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

### Mac
`brew tap osx-cross/avr`

`brew install avr-gcc avrdude`

## Circuit Diagram

In the following diagram the main components are an Arduino UNO (ATmega328p), Breadboard, Potentiometer, Micro Servo, 5 Volt Regulator, and 9 volt external power supply.  The 9 volt battery is being used to power the motor as it is not reccomended to power motors directly from the Arduino.  We use a 5 volt regulator to change the 9 volt power supply to a 5 volt power which the servo runs on.  We must ensure there is a common ground between the arduino and the external power supply.  The micro servo is then connected to groud and the 5 volt regulator.  The signal pin on the servo is connected to pin number 9 on the Arduino, or PB1 on the ATmega328p.  We must ensure the pin it is connected to is a PWM pin as this is how we will control the motor.  Available PWM pins on the ATmega328p are PD3, PD5, PD6, PB1, PB2, and PB3.  We then connect the potentiometer to positive and ground.  The Wiper pin is attached to one of the 6 analog input pins.  This allows us to get a value ranging from 0 to 1023 instead of just 0 or 1.  I used pin A0 on the Arduino or PC0 on teh Atmega.  The available Atmega pins for analog are PC0-PC5.

![Circuit Diagram](/assets/circuit_diag.png)

## Code Explanation

### Main.c

#### `Setup()`

Setup enables PC0, the analog pin, as an Input.  DDR controls the direction in which the power is expected to be traveling.  In this case we are setting DDRC, C for Port C, at PC0 to 0 making it an input from the potentiometer.  This is done by bitshifting.  The DDRC is an 8 bit register which could look something like this: 1010 0001.  The PC0 pin is the 1 bit on the furthest right, or the 0th bit.  If we want to switch this to 0, we would first just start with the number 1 which is represented as 0000 0001 and we call this a mask.  From here the 1 is already in the correct place so we shift it to the left 0 spaces (1 << 0).  Lastly, when we compare the DDRC and the maks we see that both have 1's at the PC0 spot.  To make the DDRC no longer have this we must first make the mask have a 0 at that space.  This allows us to then overwrite the 1 bit with a 0.  To do this we negate the mask which turns all of the 0's to 1's and the 1's to 0's.  This would result in a mask looking like 1111 1110.  Now we can use the & bitwise operator which will compare the DDRC to the Mask and if the bits are both 1's then the DDRC will stay a 1, if one is a 1 and the other is a 0 then the DDRC will put a 0 at the bit, and if they are both 0 then it stays 0.  This allows us to turn off the single bit and leave the rest the same.  So now we call 1010 0001 & 1111 1110 which gives us 1010 0000, effectively clearing the 0th bit and making PC0 an input.

#### `main`

main is what will be called as soon as the program starts executing.  It will first call setup to enable all of the correct pins and modes within the microcontroller.  From here it will enter a constant while loop which will continue to run for as long as the microcontroller is powered.  Inside the while loop we read in a value from the potentiometer.  We then map this value to the minimum and maximum of the servo's duty cycle to rotate it.  This logic will be explained in upcoming files.

### `Servo.c`

#### `setupTimer1()`

This method sets timer 1 to the correct settings for controlling the servo.  The Atmega328p has 3 Timers which work off the CPU clock in the microcontroller.  Timer 0 and Timer 2 are 8 bit timers allowing values between 0 and 255.  Timer 1 however is a 16 bit allowing much more fine tuning as we get values between 0 and 65,536.  For this reason we will be using Timer 1 to control the servo rotation.  First thing we do is set DDRB at pin PB1 using bitwise or (|) operator setting PB1 to 1 making it an output.  TCCR1A and TCCR1B control the settings for Timer 1.  First `TCCR1A |= (1 << COM1A1);` sets COM1A1 to 1 making the PWM non-inverting which means it starts at low, goes high, then back to low.  This also sets PB1 to use Timer 1.  From there we use `TCCR1A |= (1 << WGM11) | (1 << WGM13);` to set WGM11 and WGM13 to 1.  This indicates we will be using phase-control PWM and setting ICR1 as the TOP value.  Phase control allows us to smoothly control the pulse width of the PWM.  We then set `TCCR1B |= (1 << CS11);` which uses a prescaler of 8 dividing the timer's clock by this prescaler.  This allows us to take the 16MHz clock and lower it down to 2MHz.  This is important when dealing with lower Hz applications so we can better control the period of the PWM.  From here we determine what our ICR1 should be which will determine our Hz.  I used this equation Hz = (clock/prescaler)/(ICR1 + 1) to determine the appropriate ICR1 value.  For my servo I wanted a 50Hz or 20ms period, and plugging in our previous values we get 50Hz = (16MHz / 8)/(ICR1 + 1).  When we solve for ICR1 we get that ICR1 should be 39,999.

#### `rotateServo()`

This is a very straightforward method to rotate the servo.  OCR1A controls the duty cycle of OC1A which corresponds to PB1.  The duty cycle is the amount of time the pulse width is on or off during it's 20ms period.  So a duty cycle of 50% would mean that it is on for 10ms then off for 10ms.  Here we are simply assigning it to a specific value.

### `Adc.c`

#### `adc_init()`

This methods initializes our ADC or Analog Digital Converter.  This is built into the ATmega328p and allows us to convert analog signals into digital.  We first do this by setting the reference voltage of the ATmega.  In this case we are using 5 volts so we set REFS0 and clear REFS1 to make it 5 volts.  After this we enable the ADC by setting ADEN in the ADCSRA.  Lastly we set the ADC prescaler to 128 which bring our ADC clock to 125kHz instead of 16MHz.  We do this because the ATmega328p suggests a lower Hz operation when doing ADC for accuracy.

#### `adc_read()`

`adc_read` takes in a single parameter which is the channel or pin we want to enable.  We do this by using `ADMUX = (ADMUX & 0xF0) | (channel & 0x07);` which keeps the top 4 bits the same, then only enables the lower bit which is the channel.  We have 7 channels to choose between from ADC0 to ADC7 however only ADC0 to ADC5 are used as external analog inputs.  From here we enable the ADSC pin which triggers the hardware to start converting the signal.  We then continue to loop over this bit checking if it's one as when the conversion is done, the hardware clears this bit back to 0.  Once it is 0 then the loop exits and we return ADC which is where the converted value is stored.

#### `map_adc()`

once we get the ADC value it will be between 0 and 1023.  However, for controlling the servo motor we want this value to be between 2000 and 4000.  This is because the duty cycle for -90 degrees on the servo is when OCR1A is 2000 and 90 degrees is 4000.  This is because typical 180 deg servo motors operate between 1ms and 2ms pulses.  We calculate this by first determining the number of clock counts for a 1ms pulse width.  We do this by first taking (1ms/20ms) to get the percent of the total period.  We then take this number and multiply by the ICR1A which is the total clock counts in the 20ms pulse width.  This gives us 0.05 * 39,999 = 2000.  We then do this again for a 2ms pulse and get 4000.  This allows us to easily see that we want to have the minimum output be mapped from 0 to 2000, and we want the max ouput to be mapped from 1023 to 4000
