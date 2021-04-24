/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 04/19/21
* Project: Final Project
* File: LCD_c_file.c
* Description: This function contains the function definitions for the LCD screen. The 
* functions in this file are responsible for intializing the LCD as well as writing to it.
**************************************************************************************/





#include "LCD_h_file.h"
#include "msp.h"

/************************************************************************
* Function: LCD_init
* Inputs: None
* Outputs: None.
* Description: This function intializes the LCD with the pins connected
* to it so that future commands will be able to comunicate with the
* LCD.
************************************************************************/
void LCD_init(void){
    P6->DIR=0xF0;//sets all DBX pins as output
    P3->DIR|=BIT6;//sets the RS pin as output
    P2->DIR|=BIT3;//sets the enable pin as output
    commandWrite(3);//initialization sequence
    delay_milli(100);
    commandWrite(3);
    delay_micro(200);
    commandWrite(3);
    delay_milli(100);
    commandWrite(2);
    delay_micro(100);
    commandWrite(0x28);
    delay_micro(100);
    delay_micro(100);
    commandWrite(0x0F);
    delay_micro(100);
    commandWrite(0x01);
    delay_micro(100);
    commandWrite(0x06);
    delay_milli(10);
}
/************************************************************************
* Function: PulseEnablePin
* Inputs: None
* Outputs: None.
* Description: This function is used to pulse the enable pin with 3.3V
* for 10 microseconds which allows the LCD to update values displyed on
* the screen.
************************************************************************/
void PulseEnablePin(void){
    P2OUT &=~BIT3; // make sure pulse starts out at 0V
    delay_micro(10);
    P2OUT |=BIT3;//set the enable pin high
    delay_micro(10);//delay for 10 microseconds
    P2OUT &=~BIT3;//set the enable pin low
    delay_micro(10);
}

/************************************************************************
* Function: PushNibble
* Inputs: uint8_t nibble
* Outputs: None.
* Description: This functions purpose is to push half a byte sent from
* pushByte onto the pins connected to DB4-DB7 so that the LCD will know
* what to display on each position of the LCD.
************************************************************************/
void pushNibble(uint8_t nibble){
    P6OUT &=~0xF0; // clear P4.4-P4.7
    P6OUT |= (nibble & 0x0F) << 4; // port pins P4.4 - P4.7 wired to D4 - D7
    PulseEnablePin();//call the pulse enable function
}

/************************************************************************
* Function: PushByte
* Inputs: uint8_t byte
* Outputs: None.
* Description: This functions purpose is to push half a byte at a time
* to pushNibble. The value of the byte that the function is pushing
* comes from either commandWrite or dataWrite.
************************************************************************/
void pushByte(uint8_t byte){
    uint8_t nibble;//variable that stores each half of the byte
    nibble = (byte & 0xF0) >> 4;//stores the four most significant bits into nibble
    pushNibble(nibble);//calls push nibble and sends the first half of the byte
    nibble = byte & 0x0F;//stores the four least significant bits into nibble
    pushNibble(nibble);//calls push nibble and sends the second half of the byte
    delay_micro(100);//short delay
}

/************************************************************************
* Function: commandWrite
* Inputs: uint8_t byte
* Outputs: None.
* Description: This functions purpose is to call pushByte and pass the
* value sent from main to pushByte to begin the process of printing to
* the screen.
************************************************************************/
void commandWrite(uint8_t command){
    P3->OUT&=~BIT6;//sets the RS pin low
    pushByte(command);//calls pushByte with paramenter command
}

/************************************************************************
* Function: dataWrite
* Inputs: uint8_t byte
* Outputs: None.
* Description: This functions purpose is to call pushByte and pass the
* value sent from main to pushByte to begin the process of printing to
* the screen.
************************************************************************/
void dataWrite(uint8_t data){
    P3->OUT|=BIT6;//sets the RS pin high
    pushByte(data);//calls push byte with the parameter data
    P3->OUT&=~BIT6;//sets the RS pin low
}