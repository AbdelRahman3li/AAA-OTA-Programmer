/**
 * @file UART.c
 * @author AbdelRahman Ali
 * @brief UART Module for receiving UDS Packets  
 * @version 0.1
 * @date 2021-10-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "BOOT_MACROS.h"
#include "BOOT_UART.h"

uint8_t ReqDatalen;
uint8_t RxBuffer[255];

void ButtonPushed(uint8_t val);

void UART_Init(void){
	UCSRA |= (1<<U2X);
	UCSRB |= (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);
	UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
	UBRRH = 0;
	UBRRL = 12;
	sei();
}

void UART_SendChar(uint8_t data){
  // Wait until transmission Register Empty
  while(!(UCSRA & 1<<UDRE));
  UDR = data;
}

BOOT_ISR(USART_RXC){
	static uint8_t RxState = IDLE;
	static uint8_t bufferIdx = 0;
	uint8_t udrData;

	switch(RxState){
		case IDLE:{
			udrData = UDR;
			if(udrData == PREAMBLE){
				RxState = READY;
			}
			DEBUG_PRINT_BYTE(RxState);
			DEBUG_PRINT_BYTE(udrData);
		}break;

		case READY:{			
			ReqDatalen = UDR;
			RxState = RUNNING;
			DEBUG_PRINT_BYTE(RxState);
			DEBUG_PRINT_BYTE(ReqDatalen);
		}break;

		case RUNNING:{
			RxBuffer[bufferIdx++] = UDR;
			if(bufferIdx == ReqDatalen)
			{
				/*all data is received */
				bufferIdx = 0;
				RxState = IDLE;
				POS_RESP();
				/*call application callback*/
				PacketHandler(RxBuffer , ReqDatalen) ;
			}
			DEBUG_PRINT_BYTE(RxState);
		}break;

	}
	DEBUG_PRINT_BYTE(0xFF);
}

