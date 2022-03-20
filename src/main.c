// Switches L light (PB5) on arduino nano with signal change on PB4 (PCINT4)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <util/setbaud.h>
#include <stdio.h>

uint8_t volatile button_toggled;

void USART_Init(void)
{
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

void USART_Transmit(uint8_t data)
{
    // waits for transmit buffer
    while (!(UCSR0A & (1<<UDRE0)));

    //puts data into buffer, sends data
    UDR0 = data;
}

uint8_t USART_Receive(void)
{
    // waits data
    while (!(UCSR0A & (1<<RXC0)));

    //gets data
    return UDR0;
}

FILE USART_O = FDEV_SETUP_STREAM(USART_Transmit, NULL, _FDEV_SETUP_WRITE);
FILE USART_I = FDEV_SETUP_STREAM(NULL, USART_Receive, _FDEV_SETUP_READ);


int main(void)
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    USART_Init();
    stdout = &USART_O;
    stdin = &USART_I;



    // Sets output ports
    DDRB |= (1 << PB4);
    DDRB |= (1 << PB5);

    // sets pull up
    PORTB |= (1 << PB4);

    if(eeprom_read_byte(0))
    {
        PORTB |= (1 << PB5);
    }

    // sets PCICR to enable pin change interrupt 0
    PCICR |= (1 << PCIE0);

    // sets PCMSK0 register to enable PCINT4 as PCIE0 pin
    PCMSK0 |= (1 << PCINT4);

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

            puts("test");

            // debounce time
            _delay_ms(1000);

            button_toggled = 0;
        }
       sleep_mode();
    }
}

// handle PCI0 interrupt
ISR(PCINT0_vect)
{
    button_toggled = 1;
}

