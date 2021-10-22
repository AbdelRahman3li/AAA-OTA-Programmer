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
#include <avr/boot.h>
#include <util/crc16.h>
#include <util/delay.h>
#include "EEPROM.h"
#include <avr/pgmspace.h>
#include "BOOT_MACROS.h"
#include "BOOT_UART.h"

void BOOT_MAIN(void) __attribute__((OS_task));
void PacketHandler(uint8_t* packetData, uint8_t packetLen);
void IVTSectionSel(uint8_t IVTSection);
void flashingApp(uint8_t page, uint8_t *buf);
uint8_t CheckAppCRC(uint16_t appStartAddr, uint16_t appEndAddr, uint16_t flashedAppCRC);

static DownloadStates _downState;
static ReqStates _reqState;
static uint8_t _reqDataLen;
static uint8_t* _reqData;
static uint8_t _totalRemainsPages;
static uint8_t _currPage;

void BOOT_MAIN(){
	uint16_t _receivedAppCRC;
	UART_Init();
	POS_RESP();
	DEBUG_PRINT_BYTE(0xFF);

	while(1){
		switch(_reqState){
			case NO_REQ:{

			}break;

			case DOWNLOAD_REQ:{
				if(_downState == WAIT_DOWNREQ && _reqDataLen == 2 && _reqData[0] <= MAX_PAGES){
					_totalRemainsPages = _reqData[0];
					_currPage = 0;
					_downState = WAIT_TxDATA;
					POS_RESP();
				}else{
					_downState = WAIT_DOWNREQ;
					NEG_RESP();
				}
				DEBUG_PRINT_BYTE(_downState);
			}break;

			case TxDATA_REQ:{
				if(_downState == WAIT_TxDATA && _reqDataLen == PAGE_SIZE + 2){
					if(_currPage == _reqData[0]){
						flashingApp(_currPage, &_reqData[1]);
						++_currPage;
						_downState = --_totalRemainsPages == 0 ? WAIT_CRC_CHECK : WAIT_TxDATA;
						POS_RESP();
					}else{
						NEG_RESP();
					}
				}else{
					_downState = WAIT_DOWNREQ;
					NEG_RESP();
				}
				DEBUG_PRINT_BYTE(_downState);
			}break;

			case CRC_CHECK_REQ:{
				if(_downState == WAIT_CRC_CHECK && _reqDataLen == 3){
					_receivedAppCRC = (_reqData[0] << 8) | _reqData[1];
					--_currPage;
					if(CheckAppCRC(0, _currPage << 7, _receivedAppCRC)){
						POS_RESP();
						EEPROM_Update_Byte(APP_VALID_ADDRESS, APP_VALID);
						IVTSectionSel(APP_IVTSECTION);
						_delay_ms(150);
						__asm__ __volatile__("jmp 0x0000");
					}else{
						NEG_RESP();
						_downState = WAIT_DOWNREQ;
					}
				}else{
					NEG_RESP();
					_downState = WAIT_DOWNREQ;
				}
				DEBUG_PRINT_BYTE(_downState);
			}break;

			default:{
				NEG_RESP();
				DEBUG_PRINT_BYTE(_downState);
			}break;
		}
		_reqState = NO_REQ;
		if(!(UCSRB & (1<<RXCIE))){
			SET_BIT(UCSRB, RXCIE);
		}
	}
}

void IVTSectionSel(uint8_t IVTSection){
	/*Move IVT to start of Application*/
	if(IVTSection == APP_IVTSECTION){
		/* Enable change of interrupt vectors */
		GICR = (1<<IVCE);
		/* Move interrupts to Application section */
		CLR_BIT(GICR, IVSEL);
	}else{
		/* Enable change of interrupt vectors */
		GICR = (1<<IVCE);
		/* Move interrupts to boot Flash section */
		SET_BIT(GICR, IVSEL);
	}
}

void PacketHandler(uint8_t* packetData, uint8_t packetLen){
	CLR_BIT(UCSRB, RXCIE);
	_reqDataLen = packetLen;
	_reqState = packetData[0];
	_reqData = &packetData[1];
}

void flashingApp(uint8_t page, uint8_t *buf){
	uint8_t i;
	uint8_t sreg;
	uint16_t startPageaddress;
	uint16_t word ;

	// Disable interrupts.
	sreg = SREG;
	__asm__ __volatile__("cli");
	startPageaddress = page << 7;

	boot_page_erase_safe(startPageaddress);

	for (i=0; i < PAGE_SIZE; i+=2)
	{
		// Set up little Endian word.
		word  = *buf++;
		word += (*buf++) << 8;
		boot_page_fill_safe(startPageaddress + i, word);
	}

	boot_page_write_safe(startPageaddress);     // Store buffer in flash page.

	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.

	boot_rww_enable_safe();

	// Re-enable interrupts (if they were ever enabled).

	SREG = sreg;
}

uint8_t CheckAppCRC(uint16_t appStartAddr, uint16_t appEndAddr, uint16_t flashedAppCRC){
    uint16_t byteAddr;
    uint8_t  pgmAppByte;
    uint16_t calcCRC16 = 0xFFFF ;

    /* Compute the CRC */
    for(byteAddr = appStartAddr; byteAddr < appEndAddr; ++byteAddr)
    {
		pgmAppByte = pgm_read_byte(byteAddr);
		calcCRC16 = _crc16_update(calcCRC16, pgmAppByte);
    }

	return calcCRC16 == flashedAppCRC;
}