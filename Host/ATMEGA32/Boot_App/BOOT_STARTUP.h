#ifndef BOOT_STARTUP_H_
#define BOOT_STARTUP_H_

#include <avr/io.h>
#include <avr/common.h>

extern uint16_t _edata; //End of data section provided from Linker.
extern uint16_t _sdata; //Start of data section provided from Linker.
extern uint16_t _sbss;  //Start of bss section provided from Linker.
extern uint16_t _ebss;  //End of bss section provided from Linker.
extern uint16_t _etext; //End of text section provided from Linker.

/**
 * @brief Reset Handler used to run startup code for setting up
 * 		  the right environment for the main user code.
 * 
 */
void BOOT_IVT(void) __attribute__((naked, OS_task, section(".boot_ivt")));

/**
 * @brief ISR uses this functions name instead of vect at BOOT_ISR(vect).
 * 
 */
void RESET(void)			__attribute__((naked, interrupt));
void EXT_INT0(void)			__attribute__((interrupt, weak, alias("Default_Handler")));
void EXT_INT1(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));
void EXT_INT2(void)			__attribute__((interrupt, weak, alias("Default_Handler")));
void TIM2_COMP(void)		__attribute__((interrupt, weak, alias("Default_Handler")));
void TIM2_OVF(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));
void TIM1_CAPT(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));
void TIM1_COMPA(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));
void TIM1_COMPB(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));
void TIM1_OVF(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));
void TIM0_COMP(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));             
void TIM0_OVF(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));      
void SPI_STC(void) 			__attribute__((interrupt, weak, alias("Default_Handler")));                               
void USART_RXC(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));             
void USART_UDRE(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));           
void USART_TXC(void) 		__attribute__((interrupt, weak, alias("Default_Handler")));           
void ADC_INT(void) 			__attribute__((interrupt, weak, alias("Default_Handler")));
void EE_RDY(void) 			__attribute__((interrupt, weak, alias("Default_Handler")));           
void ANA_COMP(void)			__attribute__((interrupt, weak, alias("Default_Handler")));           
void TWI(void) 				__attribute__((interrupt, weak, alias("Default_Handler")));    
void SPM_RDY(void) 			__attribute__((interrupt, weak, alias("Default_Handler")));

/**
 * @brief a Handler used if user doesn't provide his own hanlder
 * 		  for specific enabled interrupt.
 * 
 */
void Default_Handler(void)	__attribute__((interrupt));

#endif /* BOOT_STARTUP_H_ */