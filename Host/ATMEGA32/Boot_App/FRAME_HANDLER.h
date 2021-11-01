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

#ifndef FRAME_HANDLER_H_
#define FRAME_HANDLER_H_

#include <stdint.h>

#define ACK             1   //General ACK.
#define NACK            2   //General NACK.
#define ACK_FLASHREQ     3   //FLASH REQUEST ACK.
#define NACK_FLASHREQ    4   //FLASH REQUEST NACK.
#define ACK_TxDATA      5   //TxDATA ACK.
#define NACK_TxDATA     6   //TxDATA NACK.
#define ACK_CRC_CHECK   7   //CRC ACK.
#define NACK_CRC_CHECK  8   //CRC NACK.

/**
 * @brief Init SPI which is used as communication protocol to receive frame.
 * 
 */
void FrameInit(void);

/**
 * @brief used to buffer received data to construct the frame
 * 
 */
void ReceiveFrame(void);

#endif /* FRAME_HANDLER_H_ */
