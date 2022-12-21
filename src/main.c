// Switches L light (PB5) on arduino nano with signal change on PB4 (PCINT4)
// or UART signal and then outputs state to uart transmitter

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>
#include "../lib/uart/uart.h"

uint8_t volatile toggle_led;

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
    DDRD = 0x0;

    // sets pull ups
    PORTD = (1 << PD5);

    if(eeprom_read_byte(0))
    {
        PORTB |= (1 << PB5);
    }


    // sets PCMSK2 register to enable D5 (PCINT21) as PCIE2 pin
    PCMSK2 = (1 << PCINT21);


    // interrupt (frequency) scheduler:
    //      interrupt cleans timer/counter1 and deactivates itself
    //      on clock/counter overflow ISR reactivates interrupts

    // 16bit clock1 1/64 pre-scalar 
    TCCR1B =  1 << CS11 | 1 << CS10;
    // clock counter max =~ 500ms (f_clk*scalar/2^17)

    // clear OC0A on interrupt
    TCCR1A = (1 << COM1A0) | (1 << COM1A1); 
    // T/C0 overflow A interrupt
    TIMSK1 = (1 << TOIE0);

    
    // turns on interrupts
    sei();

    while (1)
    {
        if (toggle_led)
        {
            //toggle pin B5
            PORTB ^= (1 << PB5);

            //save state on eeprom
            eeprom_update_byte(0, ((PORTB >> PB5) & 0x1));

            printstate();

            toggle_led = 0;
        }

        //checks for serial receiver buffer
        if (UCSR0A & (1<<RXC0)) {
            serial_data = getc(stdin);
            putchar(serial_data);
            putchar('\n');
            putchar('\r');

            toggle_led = 1;

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
ISR(PCINT2_vect)
{
    // if D5 is LOW
    if (!((PIND >> PIND5) & 0x1)){
        toggle_led = 1;
    }

    // sets PCICR to disable PCIE2
    PCICR &= ~(1 << PCIE2);

    // resets scheduler period count
    TCNT1H = 0;
    TCNT1L = 0;
}

// handles timer/counter1 for strict interrupt scheduler period
// by reactivating interrupts only after a certain period of 
// their last activation
ISR(TIMER1_OVF_vect)
{
    // sets PCICR to pin change interrupt enable 2
    PCICR = (1 << PCIE2);
}

//ISR(USART_RX_vect)