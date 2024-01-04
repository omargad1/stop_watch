/*
 * mini_project2.c
 *
 *  Created on: Sep 14, 2022
 *      Author: omar mohamed
 */
# include <avr/io.h>
# include<avr/interrupt.h>
# include <util/delay.h>

unsigned char sec1=0;
unsigned char sec2=0;
unsigned char min1=0;
unsigned char min2=0;
unsigned char hour2=0;
unsigned char hour1=0;

/*************************************/
void timer1(void){
	 TCCR1A=(1<<COM1A0)|(1<<FOC1A);
	 TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);
	 TCNT1 = 0;
	 OCR1A=977;
	 TIMSK=(1<<OCIE1A);

}
ISR(TIMER1_COMPA_vect){
	sec1++;
}
/************************************/
//restart timer
void int0(void){
GICR|=(1<<6);
DDRD&=~(1<<PD2);
MCUCR|=(1<<ISC01);
PORTD|=(1<<PD2);

}
ISR(INT0_vect){
	 sec1=0;
	 sec2=0;
	 min1=0;
	 min2=0;
	 hour2=0;
	 hour1=0;
}
/**************************************/
//pause timer
void int1(void){
	GICR|=(1<<7);
	DDRD&=~(1<<PD3);
	MCUCR|=(1<<ISC11)|(1<<ISC10);
}
ISR(INT1_vect){
	TCCR1B&=0;
}
/*************************************/
//resume timer
void int2(void){
	GICR|=(1<<5);
	DDRB&=~(1<<PB2);
    MCUCSR&=~(1<<ISC2);
    PORTB|=(1<<PB2);
}
ISR(INT2_vect){
	 TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);
}

int main (void){
	DDRA|=(1<<0);
	DDRC|=0X0F;
	PORTC&=0XF0;
	SREG=(1<<7);
	int0();
	int1();
	int2();
	timer1();
	while(1){
		if(sec1==10){
				sec1=0;
				sec2++;
			}
			else if((sec2==6)){
				sec1=0;
				sec2=0;
				min1++;
			}
			else if(min1>9){
				min2++;
				sec1=0;
				sec2=0;
				min1=0;
			}
			else if(min2>5){
				sec1=0;
				sec2=0;
				min1=0;
				min2=0;
				hour1++;
			}
			else if(hour1>9){
					sec1=0;
					sec2=0;
					min1=0;
					min2=0;
					hour1=0;
					hour2++;
				}
//DISPALY ON THE SEVEN SEGMENT
		PORTA=0x01;
		PORTC=(PORTC&0XF0)|(sec1&0X0F);
		_delay_ms(3);
		PORTA=0x02;
		PORTC=(PORTC&0XF0)|(sec2&0x0F);
		_delay_ms(3);
		PORTA=0x04;
		PORTC=(PORTC&0XF0)|(min1&0x0F);
		_delay_ms(3);
		PORTA=0x08;
		PORTC=(PORTC&0XF0)|(min2&0x0F);
		_delay_ms(3);
		PORTA=0x10;
		PORTC=(PORTC&0XF0)|(hour1&0x0F);
		_delay_ms(3);
		PORTA=0x20;
		PORTC=(PORTC&0XF0)|(hour2&0x0F);
		_delay_ms(3);

	}
}
