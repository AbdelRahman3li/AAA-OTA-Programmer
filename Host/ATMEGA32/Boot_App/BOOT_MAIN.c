/**
 * @file atmega32_boot_main.c
 * @author AbdelRahman Ali AbdelRazek
 * @brief BOOT MAIN for handling Burining Image Logic.
 * @version 0.1
 * @date 2021-10-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#define F_CPU 8000000UL

#include <stdint.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "EEPROM.h"
#include "BOOT_MACROS.h"
#include "FRAME_HANDLER.h"
#include "BOOT_SPI.h"

#define PAGE_SIZE	128
#define MAX_PAGES	220
 
#define APP_VALID 0x05
#define APP_VALID_ADDRESS ((uint8_t*)0x00)

#define NO_REQ			1
#define FLASH_REQ		2
#define TxDATA_REQ		3
#define CRC_CHECK_REQ	4

#define WAIT_FLASHREQ	1
#define WAIT_TxDATA		2
#define WAIT_CRC_CHECK	3

void FlashingApp(uint8_t page, uint8_t *buf);
void CheckAppReq(void);

void FlashReqHandler(uint8_t* data);
void TxDataHandler(uint8_t* data);
void CRCCheckHandler(uint8_t* data);
void FrameHandlerInit(void);

extern uint8_t reqResp; 		//Hold the response state of received frame.
static uint8_t _flashState; 	//hold the state of flashing.
static uint8_t _NoOfPages; 		//hold the number of pages to flash.
static uint8_t _currPage; 		//hold the current page waiting to be flashed.
extern uint8_t ReqDataLen; 		//hold the data length in buffer.
extern uint8_t RxBuffer[256]; 	//buffer to hold received frame.

void BOOT_MAIN() {
	CLR_BIT(DDRD, PD4); //Input Pin (LOW: BOOT MODE, HIGH: APP MODE).
	SET_BIT(PORTD, PD4); //Set Pull up resistor in PD4 (APP MODE by default).
	DDRA = 0xFF; //Set PORTA as Output for testing purpose.
	CheckAppReq();
	FrameInit();
	_flashState = WAIT_FLASHREQ; //Initialize Flashing State.
	reqResp = ACK;
	while(1){
		ReceiveFrame();
		FrameHandlerInit();
	}
}

/**
 * @brief Passes Received frame to right handler.
 * 
 */
void FrameHandlerInit(void){
	uint8_t reqState = RxBuffer[0];
	switch(reqState){
		case NO_REQ:{
			reqResp = ACK;
		}break;

		case FLASH_REQ:{
			FlashReqHandler(&RxBuffer[1]);
		}break;

		case TxDATA_REQ:{
			TxDataHandler(&RxBuffer[1]);
		}break;

		case CRC_CHECK_REQ:{
			CRCCheckHandler(&RxBuffer[1]);
		}break;

		default:{
			reqResp = NACK;
		}break;
	}
}

/**
 * @brief Initialize Flashing Parameters.
 * 
 * @param data is the received data in the frame.
 */
void FlashReqHandler(uint8_t* data){
	_NoOfPages = data[0];
	_currPage = 0;
	/*Check if Flashing state is in the right state of this request*/
	if(_flashState == WAIT_FLASHREQ){
		reqResp = ACK_FLASHREQ;
		_flashState = WAIT_TxDATA;
	}else{
		reqResp = NACK_FLASHREQ;
		_flashState = WAIT_FLASHREQ;
	}
}

/**
 * @brief Flash the received page in the Flash memory.
 * 
 * @param data page data to be flashed.
 */
void TxDataHandler(uint8_t* data){
	/*Check if Flashing state is in the right state of this request*/
	if(_flashState == WAIT_TxDATA){
	reqResp = ACK_TxDATA;
	FlashingApp(_currPage, &data[1]);
	_currPage++;
	_flashState = _currPage == _NoOfPages ? WAIT_CRC_CHECK : WAIT_TxDATA;
	}else{
		reqResp = NACK_TxDATA;
		_flashState = WAIT_FLASHREQ;
	}
}

/**
 * @brief Check the Integrity of flashed Image.
 * 		  the logic not implemented yet.			 	
 * 
 * @param data 
 */
void CRCCheckHandler(uint8_t* data){
	/*Check if Flashing state is in the right state of this request*/
	if(_flashState == WAIT_CRC_CHECK){
		reqResp = ACK_CRC_CHECK;
		//Here we have to put a function to calculate the CRC.
		/*If the CRC correct we set a Flag indicate Valid app available in EEPROM*/
		EEPROM_Update_Byte(APP_VALID_ADDRESS, APP_VALID);
		/*We jump to the beginning of the Flashed App*/
		__asm__ __volatile__("jmp 0x0000");
	}
	reqResp = NACK_CRC_CHECK;
	_flashState = WAIT_FLASHREQ;
}

/**
 * @brief Flash page logic
 * 
 * @param page page number to flash.
 * @param buf hold the page data (128 Byte).
 */
void FlashingApp(uint8_t page, uint8_t *buf){
	uint8_t i;
	uint8_t sreg;
	uint16_t startPageaddress;
	uint16_t word;
	TOGGLE_BIT(PORTA, PA7);
	// Disable interrupts.
	sreg = SREG;
	cli();
	startPageaddress = page << 7; //Page Number * Page Size (128 Byte (2^7)).

	boot_page_erase_safe(startPageaddress);

	for (i=0; i < PAGE_SIZE; i+=2)
	{
		// Set up little Endian word.
		word  = *buf++;
		word |= (*buf++) << 8;
		boot_page_fill_safe(startPageaddress + i, word);
	}

	boot_page_write_safe(startPageaddress);     // Store buffer in flash page.

	boot_rww_enable_safe();

	// Re-enable interrupts (if they were ever enabled).
	SREG = sreg;
}

/**
 * @brief Check if to jump to the App if available.
 * 
 */
void CheckAppReq(void){
	uint8_t appValid;
	appValid = EEPROM_Read_Byte(APP_VALID_ADDRESS);
	/*Check if PD4 is in APP MODE (HIGH) and EEPROM Byte Store Valid App Flag*/
	if((PIND & (1<<PIND4)) && appValid == APP_VALID){
		__asm__ __volatile__("jmp 0x0000");
	}
	SET_BIT(PORTA, PA0); //Set PA0 HIGH to Indicate Boot Mode for testing purpose.
}