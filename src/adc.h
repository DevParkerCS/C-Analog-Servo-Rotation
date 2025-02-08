#define START_ROT 2000
#define OPEN_ROT 4000
#define MIN_ADC_IN 0
#define MAX_ADC_IN 1023

void adc_init();

uint16_t adc_read(uint8_t channel);

float map_adc(uint16_t adcVal);