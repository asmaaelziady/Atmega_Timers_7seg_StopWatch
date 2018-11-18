/*
 * timerProject.c
 *
 *  Created on: Aug 14, 2018
 *      Author: A_Elziady
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define NUMBER_OF_OVERFLOWS_PER_SECOND 4
//#define MAX_NUM_OF_SECONDS 86400

unsigned char tick = 0;
//unsigned int totalTicks = 0;
unsigned char sec = 0;
unsigned char sec1 = 0;
unsigned char sec2 = 0;
unsigned char min = 0;
unsigned char min1 = 0;
unsigned char min2 = 0;
unsigned char hour = 0;
unsigned char hour1 = 0;
unsigned char hour2 = 0;
//unsigned int remaining = 0;
unsigned int count_second_flag = 0;

/* Interrupt Service Routine for timer0 compare mode */
ISR(TIMER0_COMP_vect)
{
	tick++;
	if(tick == NUMBER_OF_OVERFLOWS_PER_SECOND)
	{
		//totalTicks++;
		count_second_flag = 1;

		tick = 0; //clear the tick counter again to count a new 1 second
	}
}

/* For clock=1Mhz and prescale F_CPU/1024 every count will take 1ms
 * so we just need 250 counts to get 250ms every overflow. put initial
timer counter=0
 * and output compare register=250 0 --> 250 (250ms per overflow)
 * so we need timer to overflow 2 times to get a 0.5 second
 */

void timer0_init_CTC_mode(unsigned char t)
{
	TCNT0 = 0; //timer initial value
	OCR0  = t; //compare value
	TIMSK |= (1<<OCIE0); //enable compare interrupt
	/* Configure timer0 control register
	 * 1. Non PWM mode FOC0=1
	 * 2. CTC Mode WGM01=1 & WGM00=0
	 * 3. No need for OC0 in this example so COM00=0 & COM01=0
	 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
	 */
	TCCR0 = (1<<FOC0) | (1<<WGM01) | (1<<CS02) | (1<<CS00);
}

/*
// External INT0 enable and configuration function
void INT0_Init(void)
{
	// Enable external interrupt pin INT0
	GICR  |= (1<<INT0);
	// Trigger INT0 with the falling edge
	MCUCR &= ~(1<<ISC00);
	MCUCR |= (1<<ISC01);
}

 */
int main(void)
{
	DDRC  = 0xFF;
	DDRA  = 0xFF;
	PORTC = 0x01;
	DDRD  &= (~(1<<PD2));
	PORTD |= (1<<PD2);
	SREG  |= (1<<7);
	timer0_init_CTC_mode(250);
	//	INT0_Init();
	while(1)
	{




		if(count_second_flag == 1){


			sec1 = sec % 10;
			sec2 = sec / 10;
			min1 = min % 10;
			min2 = min / 10;
			hour1 = hour % 10;
			hour2 = hour / 10;
			sec++;
			if(sec == 60)
			{

				sec = 0;
				min++;
				//min1 = min % 10;
				//min2 = min / 10;
			}
			if(min == 60)
			{
				min = 0;
				hour++;
				//hour1 = hour % 10;
				//hour2 = hour / 10;
			}
			if(hour == 24)
			{
				hour = 0;
			}
			// reset the flag again
			count_second_flag = 0;

		}
		else
		{

			PORTC = (PORTC & 0xC0) | 0x20;
			PORTA = (PORTA & 0xF0) | sec1;

			_delay_ms(2);

			PORTC = (PORTC & 0xC0) | 0x10;
			PORTA = (PORTA & 0xF0) | sec2;

			_delay_ms(2);

			PORTC = (PORTC & 0xC0) | 0x08;
			PORTA = (PORTA & 0xF0) | min1;

			_delay_ms(2);

			PORTC = (PORTC & 0xC0) | 0x04;
			PORTA = (PORTA & 0xF0) | min2;

			_delay_ms(2);

			PORTC = (PORTC & 0xC0) | 0x02;
			PORTA = (PORTA & 0xF0) | hour1;

			_delay_ms(2);

			PORTC = (PORTC & 0xC0) | 0x01;
			PORTA = (PORTA & 0xF0) | hour2;

			_delay_ms(2);
		}



	}
	return 0;
}

