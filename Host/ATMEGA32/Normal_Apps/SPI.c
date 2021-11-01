/**
 * @file BOOT_SPI.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "SPI.h"

void SPI_InitSlave(){
	SET_BIT(DDRB, PINB6);
	SET_BIT(SPCR, SPE);
}

void SPI_SendByte(uint8_t data){
	uint8_t flushBuffer;
	SPDR = data;
	while(!(SPSR & 1<<SPIF));
	flushBuffer = SPDR;
}

uint8_t SPI_ReceiveByte(){
	while(!(SPSR & 1<<SPIF));
	return SPDR;
}