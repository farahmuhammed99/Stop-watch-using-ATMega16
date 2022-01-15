/*
 *miniproject2.c
 *
 *  Created on: Sep 13/2021
 *      Author: Farah
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char t[6]={0};
unsigned char enable=1;

void timer1_init(void){
	TCCR1A |= (1<<FOC1A) |(2<<FOC1B);
	TCCR1B |=(1<<WGM12) |(1<<CS12)|(1<<CS10);
	TCNT1 = 0 ;
	OCR1A = 976 ;
	TIMSK |= (1<<OCIE1A);



}

void int0_init(void){

	DDRD &= (~(1<<2));
	GICR |= (1<<INT0);
	MCUCR|= (1<<ISC01);
	MCUCR &= (~(1<<ISC00));
	PORTD |= (1<<2);

}
void int1_init(void){

	DDRD &= (~(1<<3));
	GICR |= (1<<INT1);
	MCUCR |= (1<<ISC11);
	MCUCR |= (1<<ISC10);


}
void int2_init(void){

	DDRB &= (~(1<<2));
	GICR |= (1<<INT2);
	MCUCSR &= (~(1<<ISC2));
	PORTB |=(1<<2);

}
ISR(INT0_vect){
	unsigned char x ;
	for(x = 0 ; x<6 ; x++){
		t[x]=0;
	}
}
ISR(INT1_vect){
	TCCR1B =0;
	TCCR1B =0;
}
ISR(INT2_vect){
	TCCR1B |=(1<<CS12);
	TCCR1B |=(1<<CS10);
	TCCR1B |=(1<<WGM12);
}

ISR(TIMER1_COMPA_vect){
	t[5]++;
	if(t[5]==10){
		t[5]=0;
		t[4]++;
		if(t[4]==6){
			t[4]=0;
			t[3]++;
			if(t[3]==10){
				t[3]=0;
				t[2]++;
				if(t[2]==6){
					t[2]=0;
					t[1]++;
					if(t[1]==10){
						t[1]=0;
						t[0]++;
						if(t[1]==4 && t[0]==2){
							t[1]=0;
							t[0]=0;
						}
					}
				}
			}
		}
	}

}

int main(void){
	DDRA = 0x3f; /* port for the 7-segments*/
	DDRC = 0x0f ; /*output port*/
	PORTC &=0xf0;
	SREG |=(1<<7);
	int0_init();
	int1_init();
	int2_init();
	timer1_init();
	unsigned char i ;
	while(1){

		for(i=0 ; i <6 ; i++){
		PORTA |= (1<<i);
		PORTC = t[i];
		_delay_ms(2);
		PORTA &= (~(1<<i));

		}

	}



}
