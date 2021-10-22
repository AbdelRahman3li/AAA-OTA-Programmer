/**
 * @file MACROS.h
 * @author AbdelRahman Ali
 * @brief Macros for handling bits.
 * @version 0.1
 * @date 2021-10-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef MACROS_H_
#define MACROS_H_

#include <avr/common.h>
#include "BOOT_UART.h"

#define DEBUG

#ifdef DEBUG
	#define DEBUG_PRINT_BYTE(MSG) UART_SendChar(MSG)
#else
	#define DEBUG_PRINT_BYTE(MSG) 
#endif
#define NULL ((void*)0)

#define SET_BIT(DATA, BIT)          ((DATA) |= (1<<BIT))
#define CLR_BIT(DATA, BIT)          ((DATA) &= ~(1<<BIT))
#define TOGGLE_BIT(DATA, BIT)       ((DATA) ^= (1<<BIT))
#define GET_BIT(DATA, BIT)          ((Data)>>(Bit)) & 1

#define PACKET_RECEIVED 1

#define REQ_APP 0x0A
#define APP_VALID 0x05
#define APP_VALID_ADDRESS ((uint8_t*)0x00)
#define APP_REQ_ADDRESS ((uint8_t*)0x01)

/*#define INIT_SP() { 	\
	SPH = RAMEND >> 8; 	\
	SPL = RAMEND; 		\
}*/
#define INIT_SP() SP = RAMEND

#define APP_IVTSECTION  0
#define BOOT_IVTSECTIOM 1

#define REQ_RECEIVED 1

#define MAX_CODE_SIZE			0x3800
#define PAGE_SIZE				128
#define MAX_PAGES				220

#define NEG_RESP()			UART_SendChar(0xF4)
#define POS_RESP()			UART_SendChar(0xEA)

typedef enum{
	NO_REQ,
	DOWNLOAD_REQ,
	TxDATA_REQ,
	CRC_CHECK_REQ
}ReqStates;

typedef enum{
	WAIT_DOWNREQ,
	WAIT_TxDATA,
	WAIT_CRC_CHECK
}DownloadStates;


#define BOOT_ISR(vect)	void vect(void) __attribute__ ((signal,__INTR_ATTRS));	\
						void vect(void)

#endif /* MACROS_H_ */
