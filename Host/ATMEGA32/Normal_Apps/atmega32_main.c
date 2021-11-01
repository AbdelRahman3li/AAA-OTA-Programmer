/**
 * @file atmega32_boot_main.c
 * @author AbdelRahman Ali
 * @brief 
 * @version 0.1
 * @date 2021-10-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "MACROS.h"

void APP_MAIN(){
	DDRD &= ~(1<<PD2);		
	PORTD |= (1<<PD2);		
	
	GICR = 1<<INT0;
	MCUCR &= ~(1<<ISC00);
	MCUCR |= (1<<ISC01);

	DDRA = 0xFF;		
	PORTA = 0xAA;
	
	sei();
	while(1){
			
	}
}

APP_ISR(EXT_INT0)
{
	PORTA ^= 0xFF;		
	_delay_ms(400);
}