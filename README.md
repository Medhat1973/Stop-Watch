#include"Kareem.h"


int main()
{
	timer1_1sec ();  // Initialize Timer1
	DDRC  = 0x0f;
	DDRA  = 0x3f;
	DDRB &= ~ 0xff;
	high ('b',0);
	high ('b',1);
	high ('b',3);
	high ('b',4);
	high ('b',5);
	high ('b',6);
	high ('b',7);
	outpin('d',4);
	outpin('d',5);
	outpin('d',0);
	INT0_fall();
	INT1_ris();
	INT2_fall();
	outpin('d',4);
	// Main loop
	while (1)
	{
		if(stop)
		{
			display();
			inc_h();
			inc_m();
			inc_s();
			dec_h();
			dec_m();
			dec_s();
		}
		else
			display();
		count_down();
	}


}


ISR(TIMER1_COMPA_vect)
{ 	low ('d',0);
	if (count ) //count up
	{
	low ('d',5);
	high ('d',4);

// Check if the unit of seconds (s0) has reached 9
if (s0 == 9) 
{
	s0 = 0;  // Reset the unit of seconds to 0
	s1++;    // Increment the tens of seconds (s1)

	// Check if the tens of seconds (s1) has reached 6 (i.e., 60 seconds)
	if (s1 == 6) 
	{
		s1 = 0;    // Reset the tens of seconds to 0 (since 60 seconds = 1 minute)
		m0++;      // Increment the unit of minutes (m0)

		// Check if the unit of minutes (m0) has reached 10
		if (m0 == 10) 
		{
			m0 = 0;  // Reset the unit of minutes to 0
			m1++;    // Increment the tens of minutes (m1)

			// Check if the tens of minutes (m1) has reached 6 (i.e., 60 minutes)
			if (m1 == 6) 
			{
				m1 = 0;    // Reset the tens of minutes to 0 (since 60 minutes = 1 hour)
				h0++;      // Increment the unit of hours (h0)
			}

			// Check if the unit of hours (h0) has reached 10 (i.e., time for tens of hours to increment)
			if (h0 == 10) 
			{
				h1++;      // Increment the tens of hours (h1)
				h0 = 0;    // Reset the unit of hours to 0
			}
		}
	}
}
else 
{
	s0++;  // Increment the unit of seconds (s0) if it has not reached 9
}

}
else
{if(s1+s0+m1+m0+h1+h0)     //count down
	{low ('d',4);
	high ('d',5);
	if (s0 == 0) 
	{
	    s0 = 9;  // Reset the unit of seconds to 9
	    if (s1 == 0)
	    {
	        s1 = 5;  // Reset the tens of seconds to 5 (since 00:00 -> 59 seconds)
	        if (m0 == 0) 
	        {
	            m0 = 9;  // Reset the unit of minutes to 9
	            if (m1 == 0) 
	            {
	                m1 = 5;  // Reset the tens of minutes to 5 (since 00:00:00 -> 59 minutes)
	                if (h0 == 0)
	                {
	                    if (h1 > 0)
	                    {
	                        h1--;   // Decrement the tens of hours (h1)
	                        h0 = 9; // Reset the unit of hours to 9
	                    }
	                } else 
	                {
	                    h0--;  // Decrement the unit of hours (h0)
	                }
	            } 
	            else	
	            {
	                m1--;  // Decrement the tens of minutes (m1)
	            }
	        }
	        else
	        {
	            m0--;  // Decrement the unit of minutes (m0)
	        }
	    } 
	    else 
	    {
	        s1--;  // Decrement the tens of seconds (s1)
	    }
	} 
	else 
	{
	    s0--;  // Decrement the unit of seconds (s0) if it's not 0
	}
}
else
{
	high ('d',0);
}

}
}


ISR(INT0_vect) //rest
{
	s1=0,s0=0,m1=0,m0=0,h1=0,h0=0;

}
ISR(INT1_vect) //pause
{
	TCCR1B &= ~( (1 << CS12) | (1 << CS11) | (1 << CS10) );  // Clear all CS bits to stop Timer1
	stop=1;
}
ISR(INT2_vect) // resume
{
	TCCR1B |= ( (1 << CS12) | (1 << CS10) );  // Clear all CS bits to stop Timer1
	stop=0;

}
