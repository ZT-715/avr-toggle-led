#include <avr/io.h>
#include <util/setbaud.h>
#include <stdio.h>


void uart_init(void){
    // sets baud rate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    // sets 2x speed
    UCSR0A |= (1<<U2X0);

    // enables receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    // sets frame format: 8data, 1stop bits
    UCSR0C = (1<<UCSZ01)|(3<<UCSZ00);
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
    // waits for transmit buffer
    while (!(UCSR0A & (1<<UDRE0)));

    //puts data into buffer, sends data
    UDR0 = data;

    return 0;
}