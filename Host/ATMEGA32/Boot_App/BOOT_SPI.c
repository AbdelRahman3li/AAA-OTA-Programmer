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

#include <avr/interrupt.h>
#include "BOOT_SPI.h"
/**
 * @brief Init SPI in slave mode, LSBFIRST, MODE1.
 * 
 */
void SPIInitSlave(){
	DDRB &= ~((1<<PB5) | (1<<PB4) | (1<<PB7));
	DDRB |= (1<<PB6);
	SPCR &= ~((1<<SPR1) | (1<<SPR0) | (1<<SPIE)  | (1<<MSTR) | (1<<CPOL));
	SPCR |=  ((1<<SPE) | (1<<CPHA) | (1<<DORD));
}
 /**
  * @brief Transceiver Send and Receive at same time.
  * 
  * @param data data to send.
  * @return uint8_t received data.
  */
uint8_t SPITxRxByte(uint8_t data){
	uint8_t receivedData;
	SPDR = data;
	while(!(SPSR & (1<<SPIF))); // Wait until receive data from master (SPIF become HIGH).
	receivedData = SPDR;
	return receivedData;
}