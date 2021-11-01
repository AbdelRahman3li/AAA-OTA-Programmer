/**
 * @file BOOT_SPI.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef BOOT_SPI_H_
#define BOOT_SPI_H_

#include <stdint.h>
#include <avr/io.h>
#include "MACROS.h"

void SPI_InitSlave();
uint8_t SPI_ReceiveByte();
void SPI_SendByte(uint8_t data);

#endif /* BOOT_SPI_H_ */