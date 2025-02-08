#include "avrincludes.h"

// Set Timer1 to phase-control PWM for servo control
void setupTimer1() {
    // Set pin 9 to OUTPUT
    DDRB |= (1 << PB1);

    // Set PWM to non-inverting
    TCCR1A |= (1 << COM1A1);

    // Set Timer 1 to phase-control PWM with ICR1 as the TOP
    TCCR1A |= (1 << WGM11) | (1 << WGM13);
    TCCR1B |= (1 << WGM11) | (1 << WGM13);

    // Divide 16MHz clock by 8 to get 2MHz clock
    // This is better for Servo control
    TCCR1B |= (1 << CS11);

    // The period of the PWM.  50Hz or 20ms periods
    ICR1 = 39999;
}

// Set duty cycle of the PWM between 500 and 2500 for servo rotation
void rotateServo(uint16_t rot) {
    OCR1A = rot;
}