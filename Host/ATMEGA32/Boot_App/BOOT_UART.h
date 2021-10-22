/**
 * @file UART.h
 * @author AbdelRahman Ali
 * @brief 
 * @version 0.1
 * @date 2021-10-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define IDLE 		0
#define READY       1
#define RUNNING	    2

#define PREAMBLE    0x01

/**
 * @brief Intialize UART Module.
 * 
 */
void UART_Init(void);
void UART_SendChar(uint8_t data);
extern void PacketHandler(uint8_t* packetData, uint8_t packetLen);

#endif /* UART_H_ */
