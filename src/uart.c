#include "avrincludes.h"
#include "uart.h"

void UART_init() {
    // Set baud rate
    UBRR0H = (BAUD_PRESCALE >> 8); 
    UBRR0L = BAUD_PRESCALE;         

    // Enable transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(uint8_t data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1<<UDRE0)));
     // Load data into the buffer
    UDR0 = data;
}