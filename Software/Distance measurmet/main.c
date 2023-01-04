/*
 * Distance measurmet.c
 *
 * Created: 1/4/2023 11:33:43 AM
 * Author : Ahmed Hassan
 */ 

#include <avr/io.h>
#include "LCD.h"
#include "std_macros.h"
#define F_CPU 1000000UL
#include <util/delay.h>
unsigned short a,b,width,Distance ; 
int main(void)
{
	LCD_intial();
	SET_BIT(DDRD,5);/*for triggering pin */
	//LCD_send_string("Distance=");
    while (1) 
    {
		/* configure triggering pin*/
		SET_BIT(PORTD,5);
	    _delay_us(10);
	    CLR_BIT(PORTD,5);
		SET_BIT(TIFR,ICF1);/*clear flag bit */
		TCCR1B = 0xc1;/* Rising edge, no prescaler , noise canceler*/
		while(READ_BIT(TIFR,ICF1)==0);
		a = ICR1 ; /* Take value of capture register */
		SET_BIT(TIFR,ICF1);/*clear flag bit */
		TCCR1B = 0x81;/* Falling edge, no prescaler , noise canceler*/
		while(READ_BIT(TIFR,ICF1)==0);
		b = ICR1 ; /* Take value of capture register */
		TCCR1B = 0; /* Stop the timer */
		width= b-a ;
		Distance = ((width*34600)/(F_CPU*2)) ;
		if (Distance>80)
		{
			LCD_clear_screen();
			LCD_send_string("Out of range");
			_delay_ms(200);
		}
		else
		{
			LCD_clear_screen();
			LCD_send_string("Distance=");
			LCD_move_cursor(1,10);
			LCD_send_char((Distance/10)+48);
			LCD_send_char((Distance%10)+48);
			LCD_send_string("cm");
			_delay_ms(200);
		}
		
		
    }
}

