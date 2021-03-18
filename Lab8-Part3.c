/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 03/18/21
* Project: Lab 8
* File: Lab8-Part3.c
* Description: Part three of this lab activity modifies the source code from part two
* in such a way that the keypad can be included in the circuit and used to control
* the variable that controls the duty cycle of the PWM signal.
**************************************************************************************/
#include "msp.h"

void main(void)
{
    keypad_init();

P2->SEL0|=BIT7;//sets up P2.7 as in/out
P2->SEL1&=~BIT7;
P2->DIR|=BIT7;//sets P2.7 as output


TIMER_A0->CCR[0]=50000-1;//sets the period of timer A
TIMER_A0->CCTL[4]=0x40;
TIMER_A0->CTL=0x0234;

int keey;

while(1){

    keey=keypad_getkey();//function call to get keypad input from user

    if(keey){

    if((keey!=10)&&(keey!=12)&&(keey!=11)){//iterates if the key pressed is 1-9
        TIMER_A0->CCR[4]=5555*keey;}//sets the on time of the PWM

    if(keey==10){}//used to capture * input

    if(keey==12){}//used to capture # input

    if(keey==11){
        keey=0;
        TIMER_A0->CCR[4]=5555*keey;}//sets the on time of the PWM
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
* Outputs: None.
* Description: This functions purpose is to dealy the program by a
* certain number of milliseconds depending upon what value is passed to
* the function
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
