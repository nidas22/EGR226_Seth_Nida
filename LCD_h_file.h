/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 04/19/21
* Project: Final Project
* File: LCD_h_file.h
* Description: This file contains the function prototypes for my LCD library
**************************************************************************************/




#include "msp.h"

#ifndef LCD_H_FILE_H_
#define LCD_H_FILE_H_

void LCD_init(void);
void PulseEnablePin(void);
void pushNibble(uint8_t nibble);
void pushByte(uint8_t byte);
void commandWrite(uint8_t command);
void dataWrite(uint8_t data);

#endif