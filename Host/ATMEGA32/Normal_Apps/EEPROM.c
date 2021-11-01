#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/common.h>
#include "EEPROM.h"
#include "MACROS.h"

uint8_t EEPROM_Read_Byte(uint8_t* dataAddr){
    uint8_t sreg;
    /*Wait for completion of previous write*/
	while(EECR & (1<<EEWE));
	/*Put address of data to be read in register*/
	EEAR = dataAddr;
	/*Start EEPROM read by writing EEMWE*/
	sreg = SREG;
    __asm__ __volatile__("cli" ::);
	SET_BIT(EECR, EERE);
    SREG = sreg;
	/*Return data from  data register*/
	return EEDR;
}
void EEPROM_Update_Byte(uint8_t* dataAddr, uint8_t data){
    uint8_t sreg;
    while(EECR & (1<<EEWE));
	/*Put address in address register*/
	EEAR = dataAddr;
	/*Put data in data register*/
	EEDR = data;

    sreg = SREG;
	__asm__ __volatile__("cli" ::);
	/*Preparing write operation by setting logical one to EEMWE*/
	CLR_BIT(EECR, EEWE);
	SET_BIT(EECR, EEMWE);
	/*Starting write operation*/
	SET_BIT(EECR, EEWE);
    SREG = sreg;
}
