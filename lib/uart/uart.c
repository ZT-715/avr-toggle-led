#include <avr/io.h>
#include <util/setbaud.h>
#include <stdio.h>


void uart_init(void){
    // sets baud rate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    // Usart Control and Status Register
    // sets 2x speed and clear flags
    UCSR0A = (1<<U2X0);

    // Receiver and Transmitter Enable
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    // sets frame format:
    // asynchronous mode
    // no parity
    // 8 data bits
    // 1 stop bit
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

unsigned char uart_receive(FILE* stream)
{
    // waits data
    while (!(UCSR0A & (1<<RXC0)));

    //gets data
    return UDR0;
}

int uart_transmit(unsigned char data, FILE* stream)
{
    // waits for transmission buffer clearance
    while (!(UCSR0A & (1<<UDRE0)));

    //puts data into buffer, sends data
    UDR0 = data;

    return 0;
}