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
#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>

uint8_t EEPROM_Read_Byte(uint8_t* dataAddr);
void EEPROM_Update_Byte(uint8_t* dataAddr, uint8_t data);

#endif /* UART_H_ */