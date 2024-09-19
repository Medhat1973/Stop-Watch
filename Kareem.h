/*
 * KAREEM.h
 *
 *  Created on: Aug 25, 2024
 *      Author: Kareem
 */

#ifndef KAREEM_H_
#define KAREEM_H_
unsigned char s1=0,s0=0,m1=0,m0=0,h1=0,h0=0,stop=0, count=1;
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define delay  _delay_ms

void outpin(char re, int bit)
{

	switch (re)
	{
	case 'a':
		DDRA |=(1<<bit);
		break;
	case 'b':
		DDRB |=(1<<bit);
		break;
	case 'c':
		DDRC |=(1<<bit);
		break;
	case 'd':
		DDRD |=(1<<bit);
	}
	return;
}


void inpin(char re, int bit)
{
	switch (re)
	{
	case 'a':
		DDRA &= ~(1<<bit);
		break;
	case 'b':
		DDRB &= ~(1<<bit);
		break;
	case 'c':
		DDRC &= ~(1<<bit);
		break;
	case 'd':
		DDRD &= ~(1<<bit);
	}
}


void high(char re, int bit) {
	switch (re) {
	case 'a':
		PORTA |= (1 << bit);  // Set bit high in PORTA
		break;
	case 'b':
		PORTB |= (1 << bit);  // Set bit high in PORTB
		break;
	case 'c':
		PORTC |= (1 << bit);  // Set bit high in PORTC
		break;
	case 'd':
		PORTD |= (1 << bit);  // Set bit high in PORTD
		break;
	}
}

void low(char re, int bit) {
	switch (re) {
	case 'a':
		PORTA &= ~(1 << bit);  // Set bit low in PORTA
		break;
	case 'b':
		PORTB &= ~(1 << bit);  // Set bit low in PORTB
		break;
	case 'c':
		PORTC &= ~(1 << bit);  // Set bit low in PORTC
		break;
	case 'd':
		PORTD &= ~(1 << bit);  // Set bit low in PORTD
		break;
	}
}

void INT0_ris(void) // for PD2 rising edge
{
	DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
	MCUCR |= (1<<ISC00) | (1<<ISC01);   // Trigger INT0 with the rising edge
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}
void INT0_fall(void)  // Configure for PD2 falling edge
{
	DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin

	// Trigger INT0 with the falling edge
	MCUCR |= (1<<ISC01);                // Set ISC01 to 1
	MCUCR &= ~(1<<ISC00);               // Clear ISC00 (set it to 0)

	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
	SREG  |= (1<<7);                    // Enable interrupts by setting the I-bit
}


void INT1_fall(void)  // for PD3 falling edge
{
	DDRD  &= (~(1<<PD3));  // Configure INT1/PD3 as input pin ISC10 = 1
	// Trigger INT1 with the falling edge
	MCUCR |= (1<<ISC11);
	MCUCR &= ~(1<<ISC10);
	GICR  |= (1<<INT1);    // Enable external interrupt pin INT1
	SREG  |= (1<<7);       // Enable interrupts by setting I-bit
}
void INT1_ris(void)  // Configure for PD3 rising edge
{
	DDRD  &= (~(1<<PD3));  // Configure INT1/PD3 as input pin

	// Trigger INT1 with the rising edge
	MCUCR |= (1<<ISC11) | (1<<ISC10);  // Set both ISC11 and ISC10 to 1

	GICR  |= (1<<INT1);    // Enable external interrupt pin INT1
	SREG  |= (1<<7);       // Enable interrupts by setting the I-bit
}


void INT2_ris(void) // for PB2 rising edge
{
	DDRB   &= (~(1<<PB2));   // Configure INT2/PB2 as input pin
	MCUCSR |= (1<<ISC2);     // Trigger INT2 with the rising edge
	GICR   |= (1<<INT2);     // Enable external interrupt pin INT2
	SREG   |= (1<<7);        // Enable interrupts by setting I-bit
}
void INT2_fall(void) // Configure for PB2 falling edge
{
	DDRB   &= (~(1<<PB2));    // Configure INT2/PB2 as input pin

	// Trigger INT2 with the falling edge
	MCUCSR &= ~(1<<ISC2);     // Clear ISC2 to 0 for falling edge

	GICR   |= (1<<INT2);      // Enable external interrupt pin INT2
	SREG   |= (1<<7);         // Enable interrupts by setting I-bit
}
void seg (unsigned char num) {
	// Ensure the digit is between 0 and 9
	if (num > 9) {
		return;  // Invalid input, do nothing
	}

	// Set PC0, PC1, PC2, PC3 as output for the BCD input to the 7447 decoder
	DDRC |= 0x0F;  // Set PC0-PC3 as output, keeping other pins unchanged

	// Send the BCD value of the digit to PC0, PC1, PC2, PC3
	PORTC = (PORTC & 0xF0) | (num & 0x0F);  // Clear PC0-PC3 and set the lower 4 bits

	// 7447 decoder will now drive the 7-segment display to show the correct digit
}
void timer1_1sec ()
{
	TCNT1 = 0;//initializing the counter register with zero
	TCCR1A = (1 << FOC1A);//setting Force Output Compare for Compare unit A
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);//setting Prescaler = 1024 and activating clock mode as CTC
	OCR1A = 15624;//top value
	TIMSK |= (1 << OCIE1A);//Output Compare A Match Interrupt Enable
	SREG |= (1<<7);
}
void display() // displayin 7 segment bi decoder 7447
{high('a', 5);
seg(s0);
delay(2);
low('a', 5);

// Display the tens of seconds (s1) on the 7-segment display connected to pin A4
high('a', 4);
seg(s1);
delay(2);
low('a', 4);

// Display the unit of minutes (m0) on the 7-segment display connected to pin A3
high('a', 3);
seg(m0);
delay(2);
low('a', 3);

// Display the tens of minutes (m1) on the 7-segment display connected to pin A2
high('a', 2);
seg(m1);
delay(2);
low('a', 2);

// Display the unit of hours (h0) on the 7-segment display connected to pin A1
high('a', 1);
seg(h0);
delay(2);
low('a', 1);

// Display the tens of hours (h1) on the 7-segment display connected to pin A0
high('a', 0);
seg(h1);
delay(2);
low('a', 0);
}
void inc_h()// increment hours
{
	if (!(PINB & (1<<1)))
	{
		h0++;
		if (h0 == 10)
		{
			h1++;      // Increment the tens of hours (h1)
			h0 = 0;    // Reset the unit of hours to 0
		}
		while(!(PINB & (1<<1)))
		{display();}
	}

}

void dec_h() // decrement hours
{
    if (!(PINB & (1<<0))) // Assuming you're using PB0 for decrement
    {
        if (h0 == 0)
        {
            if (h1 > 0)
            {
                h1--;      // Decrement the tens of hours (h1)
                h0 = 9;    // Set the units of hours to 9
            }
        }
        else
        {
            h0--;          // Decrement the units of hours (h0)
        }

        while(!(PINB & (1<<0)))
        {
            display();      // Keep displaying until the button is released
        }
    }
}

void inc_m()// increment  minute
{
	if (!(PINB & (1<<4)))
	{
		m0++;
		if (m0 == 10) {
			m0 = 0;  // Reset the unit of minutes to 0
			m1++;    // Increment the tens of minutes (m1)

			// Check if the tens of minutes (m1) has reached 6 (i.e., 60 minutes)
			if (m1 == 6) {
				m1 = 0;    // Reset the tens of minutes to 0 (since 60 minutes = 1 hour)
				h0++;      // Increment the unit of hours (h0)
			}

			// Check if the unit of hours (h0) has reached 10 (i.e., time for tens of hours to increment)
			if (h0 == 10) {
				h1++;      // Increment the tens of hours (h1)
				h0 = 0;    // Reset the unit of hours to 0
			}
		}
	}
	while(!(PINB & (1<<4)))
	{display();}
	// Reset the unit of hours to 0


}
void dec_m() // decrement minutes
{
    if (!(PINB & (1<<3))) // Assuming PB3 is for decrementing minutes
    {
        if (m0 == 0)
        {
            if (m1 > 0)
            {
                m1--;      // Decrement the tens of minutes (m1)
                m0 = 9;    // Set the units of minutes to 9
            }
        }
        else
        {
            m0--;          // Decrement the units of minutes (m0)
        }

        while (!(PINB & (1<<3)))
        {
            display();      // Wait for the button to be released
        }
    }
}

void inc_s()// increment  minute
{
	if (!(PINB & (1<<6)))
	{ s0++;
	if (s0 == 9) {
		s0 = 0;  // Reset the unit of seconds to 0
		s1++;    // Increment the tens of seconds (s1)

		// Check if the tens of seconds (s1) has reached 6 (i.e., 60 seconds)
		if (s1 == 6) {
			s1 = 0;    // Reset the tens of seconds to 0 (since 60 seconds = 1 minute)
			m0++;      // Increment the unit of minutes (m0)

			// Check if the unit of minutes (m0) has reached 10
			if (m0 == 10) {
				m0 = 0;  // Reset the unit of minutes to 0
				m1++;    // Increment the tens of minutes (m1)

				// Check if the tens of minutes (m1) has reached 6 (i.e., 60 minutes)
				if (m1 == 6) {
					m1 = 0;    // Reset the tens of minutes to 0 (since 60 minutes = 1 hour)
					h0++;      // Increment the unit of hours (h0)
				}

				// Check if the unit of hours (h0) has reached 10 (i.e., time for tens of hours to increment)
				if (h0 == 10) {
					h1++;      // Increment the tens of hours (h1)
					h0 = 0;    // Reset the unit of hours to 0
				}
			}
		}
	} }




	while(!(PINB & (1<<6)))
	{display();}
	// Reset the unit of hours to 0

}


void dec_s() // decrement seconds
{
    if (!(PINB & (1<<5))) // Assuming PB5 is for decrementing seconds
    {
        if (s0 == 0)
        {
            if (s1 > 0)
            {
                s1--;      // Decrement the tens of seconds (s1)
                s0 = 9;    // Set the units of seconds to 9
            }
        }
        else
        {
            s0--;          // Decrement the units of seconds (s0)
        }

        while (!(PINB & (1<<5)))
        {
            display();      // Wait for the button to be released
        }
    }
}
void count_down()
{


	if (!(PINB & (1<<7)))
	{count ^=1;

		while(!(PINB & (1<<7)))
		{display();}
	}

}

#endif /* KAREEM_H_ */
