/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 03/02/21
* Project: Lab 6
* File: Lab6-Part1.c
* Description: Part one of this lab is too connect the keypad to the msp and configure
* the msp to print the value pressed on the key pad to the console window each time a
* key is pressed.
**************************************************************************************/
#include "msp.h"
#include <stdio.h>

int read_keypad(int column);
void print(int keey);

void main(void)
{

    int keey;
  keypad_init();

    while(1){
        keey=keypad_getkey();//function call to get keypad input from user

        if(keey){

        if((keey!=10)&&(keey!=12)&&(keey!=11)){//prints the value of the key pressed to the console window if 1-9
        printf("key entered [%d]\n",keey);}

        if(keey==10){printf("key entered [*]\n");}//prints the value of the key pressed to the console window if *

        if(keey==12){printf("key entered [#]\n");}//prints the value of the key pressed to the console window if #

        if(keey==11){printf("key entered [0]\n");}//prints the value of the key pressed to the console window if 0
        }
    }
}
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
    SysTick_delay(10);
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

/************************************************************************
* Function: SysTick_delay
* Inputs: uint16_t delay
* Outputs: none
* Description: this function is used to delay the program by whatever
* number of milliseconds that is passed to it.
************************************************************************/
void SysTick_delay(uint16_t delay){
int i;
SysTick->LOAD=3000-1;
SysTick->VAL =0;
SysTick->CTRL=0x5;
for(i=0;i<delay;i++){
    while((SysTick->CTRL & 0x10000)==0){}
}
SysTick->CTRL=0;
}

