#ifndef UART_H

#include <stdio.h>

void uart_init(void);

unsigned char uart_receive(FILE* stream);

int uart_transmit(unsigned char data, FILE* stream);

FILE UART_O = FDEV_SETUP_STREAM(uart_transmit, NULL, _FDEV_SETUP_WRITE);
FILE UART_I = FDEV_SETUP_STREAM(NULL, uart_receive, _FDEV_SETUP_READ);

#endif