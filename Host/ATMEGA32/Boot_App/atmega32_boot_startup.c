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

#include <stdint.h>
#include "EEPROM.h"
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "BOOT_MACROS.h"

extern uint16_t _edata;
extern uint16_t _sdata;
extern uint16_t _sbss;
extern uint16_t _ebss;
extern uint16_t _etext;

void BOOT_IVT(void) __attribute__((naked, OS_task, section(".boot_ivt")));

void RESET(void)			__attribute__((naked, signal));
void EXT_INT0(void)			__attribute__((signal, weak, alias("Default_Handler")));
void EXT_INT1(void) 		__attribute__((signal, weak, alias("Default_Handler")));
void EXT_INT2(void)			__attribute__((signal, weak, alias("Default_Handler")));
void TIM2_COMP(void)		__attribute__((signal, weak, alias("Default_Handler")));
void TIM2_OVF(void) 		__attribute__((signal, weak, alias("Default_Handler")));
void TIM1_CAPT(void) 		__attribute__((signal, weak, alias("Default_Handler")));
void TIM1_COMPA(void) 		__attribute__((signal, weak, alias("Default_Handler")));
void TIM1_COMPB(void) 		__attribute__((signal, weak, alias("Default_Handler")));
void TIM1_OVF(void) 		__attribute__((signal, weak, alias("Default_Handler")));
void TIM0_COMP(void) 		__attribute__((signal, weak, alias("Default_Handler")));             
void TIM0_OVF(void) 		__attribute__((signal, weak, alias("Default_Handler")));      
void SPI_STC(void) 			__attribute__((signal, weak, alias("Default_Handler")));                               
void USART_RXC(void) 		__attribute__((signal, weak, alias("Default_Handler")));             
void USART_UDRE(void) 		__attribute__((signal, weak, alias("Default_Handler")));           
void USART_TXC(void) 		__attribute__((signal, weak, alias("Default_Handler")));           
void ADC_INT(void) 			__attribute__((signal, weak, alias("Default_Handler")));
void EE_RDY(void) 			__attribute__((signal, weak, alias("Default_Handler")));           
void ANA_COMP(void)			__attribute__((signal, weak, alias("Default_Handler")));           
void TWI(void) 				__attribute__((signal, weak, alias("Default_Handler")));    
void SPM_RDY(void) 			__attribute__((signal, weak, alias("Default_Handler")));

void Default_Handler(void)	__attribute__((signal));

void IVTSectionSel(uint8_t IVTSection);
void BOOT_MAIN(void);

/**
 * @brief an Array contains Interrupt Vector Table for ATMEGA32
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
 * @brief a Handler used if user doesn't provide his own hanlder
 * 		  for specific enabled interrupt.
 * 
 */
void Default_Handler(void){
}

/**
 * @brief Reset Handler used to run startup code for setting up
 * 		  the right environment for the main user code.
 * 
 */
void RESET(void){
	INIT_SP();
	sei();
	uint8_t validAppFlag, reqAppFlag;
	reqAppFlag = EEPROM_Read_Byte(APP_REQ_ADDRESS);
	validAppFlag = reqAppFlag ==  REQ_APP ? 0 : EEPROM_Read_Byte(APP_VALID_ADDRESS);
	if(validAppFlag == APP_VALID){
		IVTSectionSel(APP_IVTSECTION);
		_delay_ms(150);
		__asm__ __volatile__("jmp 0x0000");
	}

	IVTSectionSel(BOOT_IVTSECTIOM);
	_delay_ms(150);

	//copy .data section to SRAM
	uint16_t size = (uint16_t)&_edata - (uint16_t)&_sdata;
	uint8_t *pDst = (uint8_t*)&_sdata; //sram
	uint8_t *pSrc = (uint8_t*)&_etext; //flash
	for(uint16_t i = 0; i < size; ++i)
	{
		*pDst++ = *pSrc++;
	}
	
	//Init. the .bss section to zero in SRAM
	size = (uint16_t)&_ebss - (uint16_t)&_sbss;
	pDst = (uint8_t*)&_sbss;
	for(uint16_t i = 0; i < size; ++i)
	{
		*pDst++ = 0;
	}

	INIT_SP();
	__asm__ __volatile__("jmp BOOT_MAIN");
	
}