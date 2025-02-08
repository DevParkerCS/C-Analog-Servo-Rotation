#include "avrincludes.h"
#include "adc.h"

void adc_init() {
    // Set Reference voltage
    ADMUX |= (1 << REFS0);  
    ADMUX &= ~(1 << REFS1); 

    // Select ADC0 (PC0/A0)
    ADMUX &= 0xF0; 
    
    // Enable ADC
    ADCSRA |= (1 << ADEN);
    // Set prescaler to 128 (16MHz/128 = 125kHz)
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}

uint16_t adc_read(uint8_t channel) {
    // Select ADC channel (0-7)
    ADMUX = (ADMUX & 0xF0) | (channel & 0x07); 

    ADCSRA |= (1 << ADSC);  // Start conversion

    while (ADCSRA & (1 << ADSC)){};  // Wait for conversion to complete

    return ADC;  // Read the ADC value (10-bit result)
}

float map_adc(uint16_t adcVal) {
    // Convert constants to float for precise calculation
    return ((float)(adcVal - MIN_ADC_IN) * (OPEN_ROT - START_ROT) / (MAX_ADC_IN - MIN_ADC_IN)) + START_ROT;
}
