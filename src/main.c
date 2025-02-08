#include "avrincludes.h"
#include "uart.h"
#include "adc.h"
#include "servo.h"
#include <stdio.h>

// Setup pins and hardware
void setup() {
    // PC0 as input
    DDRC &= ~(1 << PC0);

    setupTimer1();
    UART_init();
    adc_init();
}

int main() {
    setup();
    // Constantly read analog signal then rotate the servo according to the input
    while (1) {
        uint16_t val = adc_read(0);
        rotateServo(map_adc(val));
    }
}