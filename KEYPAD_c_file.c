/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 04/19/21
* Project: Final Project
* File: KEYPAD_c_file.c
* Description: This library contains the function definitions responsible for initializing
* and collecting input from the keypad.
**************************************************************************************/

#include "KEYPAD_h_file.h"
#include "msp.h"

/************************************************************************
* Function: keypad_init
* Inputs: None
* Outputs: None.
* Description: This function initializes the pins connected to the
* keypad and sets them to high.
************************************************************************/
void keypad_init(void){
    P4->DIR&=~0xFF;//sets all pins connected to input
    P4->REN|=0x0F;//enables pull up resistor
    P4->OUT|=0x0F;//sets the rows to high
}

/************************************************************************
* Function: keypad_getkey
* Inputs: None
* Outputs: None.
* Description: This function was sourced from the lecture slides. The
* purpose of this function is to detect if a button on the keypad is
* pressed and send the information regarding the button press back
* to main.
************************************************************************/
int keypad_getkey(void){
    int row,col;
for(col=0;col<3;col++){//for loop used to detect if any buttons have been pressed
    P4->DIR=0x00;//sets all columns to inputs
    P4->DIR|=BIT(4+col);//set current column to output
    P4->OUT&=~BIT(4+col);//set current column to low
    delay_milli(10);
    row=P4->IN & 0x0F;//reads the value of all rows
    while(!(P4IN & BIT0)|!(P4IN & BIT1)|!(P4IN & BIT2)|!(P4IN & BIT3));//iterates while a button is pressed
    if(row!= 0x0F){break;}
}
P4->DIR = 0x00;//sets columns to input
if(col == 3) return 0;//theses next few lines return the button that has been pressed back to main
if(row == 0x0E) return col+1;
if(row == 0x0D) return col+4;
if(row == 0x0B) return col+7;
if(row == 0x07) return col+10;
}
