// Switches L light (PB5) on arduino nano with signal change on PB4 (PCINT4)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t volatile button_toggled;

int main(void)
{
    // Sets PORT B.5 as an output
    DDRB |= (1 << PB5);

    // sets PB5 HIGH (pull up)
    //PORTB |= (1 << PB5);

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
            PORTB ^= (1 << PB5);

            // debounce time
            _delay_ms(1500);

            button_toggled = 0;
        }
    }
}

// handle PCI0 interrupt
ISR(PCINT0_vect, ISR_BLOCK)
{
    button_toggled = 1;
}