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
#include "BOOT_MACROS.h"

/**
 * @brief Init SPI in slave mode, LSBFIRST, MODE1.
 * 
 */
void SPIInitSlave();

/**
  * @brief Transceiver Send and Receive at same time.
  * 
  * @param data data to send.
  * @return uint8_t received data.
  */
uint8_t SPITxRxByte(uint8_t data);

#endif /* BOOT_SPI_H_ */