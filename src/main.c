// Switches L light (PB5) on arduino nano with signal change on PB4 (PCINT4)
// or UART signal and then outputs state to uart transmitter

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>
#include "../lib/uart/uart.h"

uint8_t volatile button_toggled;

void printstate(void);

int main(void)
{
    //set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    uart_init();
    stdout = &UART_O;
    stdin = &UART_I;

    // Sets output ports
    DDRB = (1 << PB4)|(1 << PB5);

    // sets pull up
    PORTB = (1 << PB4);

    if(eeprom_read_byte(0))
    {
        PORTB |= (1 << PB5);
    }

    // sets PCICR to enable pin change interrupt 0
    PCICR = (1 << PCIE0);

    // sets PCMSK0 register to enable PCINT4 as PCIE0 pin
    PCMSK0 = (1 << PCINT4);

    volatile char serial_data;

    // turns on interrupts
    sei();

    while (1)
    {
        if (button_toggled)
        {
            //toggles pin b 5
            PORTB ^= (1 << PB5);

            //save state on eeprom
            eeprom_update_byte(0, ((PORTB >> PB5) & 0x1));

            printstate();

            // debounce time
            _delay_ms(1000);

            button_toggled = 0;
        }

        //checks for serial receiver buffer
        if(UCSR0A & (1<<RXC0))
        {
            serial_data = getc(stdin);
            putchar(serial_data);
            putchar('\n');
            putchar('\r');

            //toggles pin b 5
            PORTB ^= (1 << PB5);

            eeprom_update_byte(0, ((PORTB >> PB5) & 0x1));

            printstate();

            while ((UCSR0A & (1<<RXC0))) serial_data = UDR0;
            serial_data = 0;
        }

        //sleep_mode();
    }
}

void printstate(void)
{
    if((PORTB >> PB5) & 0x1){
        puts("on\n\r");
    }
    else{
        puts("off\n\r");
    }
}

// handle PCI0 interrupt
ISR(PCINT0_vect)
{
    button_toggled = 1;
}
//ISR(USART_RX_vect)

