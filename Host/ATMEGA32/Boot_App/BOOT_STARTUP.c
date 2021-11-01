/**
 * @file atmega32_boot_startup.c
 * @author AbdelRahman Ali
 * 
 * @brief Startup code used to:
 * 		    1-Define IVT.
 *			2-Setting the right environment.
				2.1-Initialize .data and .bss sections.
				2.2-Initialize stack pointer.
				2.3-Jump to the main routine.
 *
 * @version 0.1
 * @date 2021-10-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#define F_CPU 8000000UL

#include <stdint.h>
#include "EEPROM.h"
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/io.h>
#include "BOOT_MACROS.h"
#include "BOOT_STARTUP.h"



/**
 * @brief a Naked function contains Interrupt Vector Table of jmp instructions for ISR.
 * 
 */

void BOOT_IVT(void){
	__asm__ __volatile__("jmp RESET");
 	__asm__ __volatile__("jmp EXT_INT0");
 	__asm__ __volatile__("jmp EXT_INT1");
 	__asm__ __volatile__("jmp EXT_INT2");
 	__asm__ __volatile__("jmp TIM2_COMP");
 	__asm__ __volatile__("jmp TIM2_OVF");
	__asm__ __volatile__("jmp TIM1_CAPT");
	__asm__ __volatile__("jmp TIM1_COMPA");
	__asm__ __volatile__("jmp TIM1_COMPB");
	__asm__ __volatile__("jmp TIM1_OVF");
	__asm__ __volatile__("jmp TIM0_COMP");
	__asm__ __volatile__("jmp TIM0_OVF");         
	__asm__ __volatile__("jmp SPI_STC");  
	__asm__ __volatile__("jmp USART_RXC");                         
	__asm__ __volatile__("jmp USART_UDRE");         
	__asm__ __volatile__("jmp USART_TXC");       
	__asm__ __volatile__("jmp ADC_INT");      
	__asm__ __volatile__("jmp EE_RDY");       
	__asm__ __volatile__("jmp ANA_COMP");       
	__asm__ __volatile__("jmp TWI");
	__asm__ __volatile__("jmp SPM_RDY");
}

/**
 * @brief Reset Handler used to run startup code for setting up
 * 		  the right environment for the main user code.
 * 
 */
void RESET(void){
	SP = RAMEND;
	//copy .data section to SRAM
	uint16_t i;
	uint16_t size = (uint16_t)&_edata - (uint16_t)&_sdata;
	uint8_t *pDst = (uint8_t*)&_sdata; //sram
	uint8_t *pSrc = (uint8_t*)&_etext; //flash
	for(i = 0; i < size; ++i)
	{
		*pDst = *pSrc;
		++pDst;
		++pSrc;
	}
	
	//Init. the .bss section to zero in SRAM
	size = (uint16_t)&_ebss - (uint16_t)&_sbss;
	pDst = (uint8_t*)&_sbss;
	for(i = 0; i < size; ++i)
	{
		*pDst = 0;
		++pDst;
	}
	SP = RAMEND;
	__asm__ __volatile__("jmp BOOT_MAIN"); //I preferred to use jump over call main.
	
}

/**
 * @brief a Handler used if user doesn't provide his own hanlder
 * 		  for specific enabled interrupt.
 * 
 */
void Default_Handler(void){
	reti();
}