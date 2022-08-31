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
    volatile char serial_data;

    //set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    uart_init();
    stdout = &UART_O;
    stdin = &UART_I;

    // Sets i/o ports
    DDRB = (1 << DDB5);
    DDRD = 0;

    // sets pull ups
    PORTD = (1 << PD5);

    if(eeprom_read_byte(0))
    {
        PORTB |= (1 << PB5);
    }

    // sets PCICR to enable pin change interrupt enable 2
    PCICR = (1 << PCIE2);

    // sets PCMSK2 register to enable D5 (PCINT21) as PCIE2 pin
    PCMSK2 = (1 << PCINT21);

    // turns on interrupts
    sei();

    while (1)
    {
        if (button_toggled)
        {
            //cli();

            //toggles pin b 5
            PORTB ^= (1 << PB5);

            //save state on eeprom
            eeprom_update_byte(0, ((PORTB >> PB5) & 0x1));

            printstate();

            button_toggled = 0;
        }

        //checks for serial receiver buffer
        if (UCSR0A & (1<<RXC0)) {
            serial_data = getc(stdin);
            putchar(serial_data);
            putchar('\n');
            putchar('\r');

            //toggles pin b 5
            PORTB ^= (1 << PB5);

            eeprom_update_byte(0, ((PORTB >> PB5) & 0x1));

            printstate();

            while (UCSR0A & (1<<RXC0)) serial_data = UDR0;
            serial_data = 0;
        }

        //sleep_mode();
    }
}

void printstate(void)
{
    if ((PORTB >> PB5) & 0x1) {
        puts("on\n\r");
    }
    else {
        puts("off\n\r");
    }
}

// handle PCI0 interrupt
ISR(PCINT0_vect)
{
    if (!((PIND >> PIND5) & 0x1)){
        button_toggled = 1;
    }
}

//ISR(USART_RX_vect)