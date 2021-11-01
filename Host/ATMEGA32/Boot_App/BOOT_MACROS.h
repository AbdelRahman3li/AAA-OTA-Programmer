/**
 * @file MACROS.h
 * @author AbdelRahman Ali AbdelRazek
 * @brief Macros for handling bits.
 * @version 0.1
 * @date 2021-10-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef MACROS_H_
#define MACROS_H_

#define F_CPU 8000000UL

#include <util/delay.h>
#include <avr/common.h>

#define NULL ((void*)0)

#define SET_BIT(DATA, BIT)          ((DATA) |= (1<<BIT))
#define CLR_BIT(DATA, BIT)          ((DATA) &= ~(1<<BIT))
#define TOGGLE_BIT(DATA, BIT)       ((DATA) ^= (1<<BIT))
#define GET_BIT(DATA, BIT)          ((Data)>>(Bit)) & 1

/**
 * @brief Macro to handle ISR, vect refers to ISR reference
 * 		  in Start up code for example BOOT_ISR(EXT_INT0).
 * 
 */
#define BOOT_ISR(vect)	void vect(void) __attribute__ ((interrupt));	\
						void vect(void)

#endif /* MACROS_H_ */
