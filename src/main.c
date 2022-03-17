// Switches L light (PB5) on arduino nano with signal change on PB4 (PCINT4)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/sleep.h>

uint8_t volatile button_toggled;

int main(void)
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

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