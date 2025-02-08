#define F_CPU 16000000UL  // Clock speed
#define BAUD 9600
#define BAUD_PRESCALE ((F_CPU / (16UL * BAUD)) - 1)

void UART_init();
void uart_transmit(uint8_t data);