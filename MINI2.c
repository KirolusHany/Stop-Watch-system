#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char second1=0;
unsigned char second2=0;
unsigned char minute1=0;
unsigned char minute2=0;
unsigned char hour1=0;
unsigned char hour2=0;
unsigned char tick=0;

void TIMER1_CTC_INT(void){
	TCNT1 = 0;
	OCR1A =1000;
	TIMSK |= (1<<OCIE1A);
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<CS10)|(1<<CS12);
}
ISR(TIMER1_COMPA_vect){
	tick++;
	second1++;
    if(second1==10){
    	second1=0;
    	second2++;
    }
	if(second2==6&&second1==0){
		second1=0;
		second2=0;
		minute1++;
	}
	if(minute1==10){
		minute1=0;
		minute2++;
	}
	if(minute2==6&&minute1==0){
			minute1=0;
			minute2=0;
			hour1++;
		}
	if(hour1==10){
		hour1=0;
		hour2++;
	}
	if(hour2==2&&hour1==4){
		hour1=0;
		hour2=0;
	}
}
void reset_INT0(void){
	DDRD&=~(1<<PD2);
	PORTD|=(1<<PD2);//PULL UP
	MCUCR|=(1<<ISC01);
	GICR|=(1<<INT0);
	SREG|=(1<<7);
}
ISR(INT0_vect){
	second1=0;
	second2=0;
	minute1=0;
	minute2=0;
	hour1=0;
	hour2=0;
}
void paused_INT1(void){
	DDRD&=~(1<<PD3);
	MCUCR|=(1<<ISC01)|(1<<ISC11);//raising edge
	GICR|=(1<<INT1);//module interrupt
	SREG|=(1<<7);//globle interrupt bit
}

ISR(INT1_vect){
	TIMSK &= ~(1<<OCIE1A);
}

void resumed_INT2(void){
	DDRB&=~(1<<PB2);
	PORTB|=(1<<PB2);//PULL UP
	MCUCR|=(1<<ISC01);
	GICR|=(1<<INT2);
	SREG|=(1<<7);
}
ISR(INT2_vect){
	TIMSK |= (1<<OCIE1A);
}

int main(void){
	DDRC=0X0F;
	PORTC=0XF0;
	DDRA=0XFF;
	TIMER1_CTC_INT();
	reset_INT0();
	paused_INT1();
	resumed_INT2();
   while(1){
	PORTA=(1<<5);
	PORTC=(PORTC&0XF0)|(second1&0X0F);
	_delay_ms(3);
	PORTA=(1<<4);
	PORTC=(PORTC&0XF0)|(second2&0X0F);
	_delay_ms(3);
	PORTA=(1<<3);
	PORTC=(PORTC&0XF0)|(minute1&0X0F);
	_delay_ms(3);
	PORTA=(1<<2);
	PORTC=(PORTC&0XF0)|(minute2&0X0F);
	_delay_ms(3);
	PORTA=(1<<1);
	PORTC=(PORTC&0XF0)|(hour1&0X0F);
	_delay_ms(3);
	PORTA=(1<<0);
	PORTC=(PORTC&0XF0)|(hour2&0X0F);
	_delay_ms(3);

   }
}
