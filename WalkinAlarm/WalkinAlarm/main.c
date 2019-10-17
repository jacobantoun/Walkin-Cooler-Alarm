/*
 * WalkinAlarm.c
 * Buzzer will sound when prox switch no longer detects a metal object for about 10 seconds. Buzzer will stop as soon as metal is reintroduced to prox sensor.
 * Created: 10/5/2019 12:12:24 PM
 * Author : jacob
 */ 

#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define buzzer PA1 // buzzer on PORTA1
#define prox PA0 // prox on PORTA0
#define led PA2 // test LED so I don't cry every time i hear the alarm

unsigned long timer = 0; // timer variable
int time_now; // placeholder for current time
int period = 10; // time delay for buzzer to go off

int main(void)
{
    DDRA |= (1 << led); // LED as output
	DDRA &= ~(1 << prox); // prox sensor as input
	PORTA |= (1 << prox); // pull up on prox
	DDRA |= (1 << buzzer); // buzzer as output
	
	timer1_init(); // init timer 1 in periodic interrupt mode
	
	sei(); // enable global interrupts
	
    while (1) 
    {
		if( ((PINA & (1 << 0)) == 1 ) ) // check if sensor is in contact with metal
		{
			if(timer > time_now + period) // if timer exceeds period (variable), buzzer and led = 1
			{
				// Enable the alarm
				time_now = timer; // keep track of time
				PORTA |= (1 << led); // led on
				PORTA |= (1 << buzzer); // buzzer on	
			}
		}
		else // timer within limits, all is normal
		{
			// Alarm off
			PORTA &= ~(1 << led); // led off
			PORTA &= ~(1 << buzzer); // buzzer off
			time_now = timer; // keep track of time
		}
    }
}

void timer1_init(void)
{
	TCCR1B |= (1 << WGM12); // CTC Mode on Timer 1
	TIMSK1 |= (1 << OCIE1A); // Enable CTC Interrupt
	sei(); // Enable global interrupts
	OCR1A = 15624; // set for 1Hz
	TCCR1B |= (1 << CS10) | (1 << CS11); // Prescale at Fcpu / 64
}

ISR(TIM1_COMPA_vect) 
{
	timer++; // increment timer every 1 second
}


