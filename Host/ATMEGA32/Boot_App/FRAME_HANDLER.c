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
#include "BOOT_SPI.h"
#include "FRAME_HANDLER.h"

uint8_t reqResp;
uint8_t ReqDataLen;
uint8_t RxBuffer[256];

/**
 * @brief Init SPI which is used as communication protocol to receive frame.
 * 
 */
void FrameInit(void){
	SPIInitSlave();	
}

/**
 * @brief used to buffer received data to construct the frame
 * 
 */
void ReceiveFrame(){
	uint8_t frameIdx = 0;
	ReqDataLen = SPITxRxByte(frameIdx); // First byte recieved is the frame length.
	/*Iterate until all frame data received*/
	while(frameIdx < ReqDataLen){
		RxBuffer[frameIdx] = SPITxRxByte(frameIdx + 1); //receive data and send the index of this data.
		frameIdx++;
	}
	SPITxRxByte(reqResp); // send the Response of the previous frame (Initially send ACK).
}
