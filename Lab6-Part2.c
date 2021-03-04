/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 03/02/21
* Project: Lab 6
* File: Lab6-Part2.c
* Description: Part two of this lab uses the code from part one and thakes the program
* one step further. In part two the user enters a 4 digit pin and it is stored in the
* program. Once the user presses the pound key the four most recent valid inputs are printed
* to the console window.
**************************************************************************************/
#include "msp.h"
#include <stdio.h>

int read_keypad(int column);//function to initialize the pins connected to the keypad
void print(int keey);//function to print the number to the console window

void main(void)
{
    int pin_nums[4]={-1,-1,-1,-1};//array used to hold the four digit pin
    int k=0;
    int keey;
    int h0,h1,h2,h3;//dumy variables used to transfer numbers inside the array pin_nums
  keypad_init();//initialization function call
  printf("Please enter a 4 digit pin\n");

    while(1){
        keey=keypad_getkey();//function call to get keypad input from user
        if(keey){

        if((keey!=10)&&(keey!=12)&&(keey!=11)){//if the number pressed by the user is not 0, *, or #
        printf("key entered [%d]\n",keey);
        h0=pin_nums[0];//a series of reassignments that shifts the numbers stored in pin_nums so that only the 4 most recent entries are stored
        h1=pin_nums[1];
        h2=pin_nums[2];
        h3=pin_nums[3];
        pin_nums[0]=h1;
        pin_nums[1]=h2;
        pin_nums[2]=h3;
        pin_nums[3]=keey;}

        if(keey==11){//if the key pressed is 0
        printf("key entered [0]\n");
        keey=0;
        h0=pin_nums[0];//a series of reassignments that shifts the numbers stored in pin_nums so that only the 4 most recent entries are stored
        h1=pin_nums[1];
        h2=pin_nums[2];
        h3=pin_nums[3];
        pin_nums[0]=h1;
        pin_nums[1]=h2;
        pin_nums[2]=h3;
        pin_nums[3]=keey;}

        if(keey==10){printf("key entered [*]\n");}//corresponds to the * on the keypad

        if(keey==12){printf("key entered [#]\n");//if the key pressed is the # key
        if(pin_nums[0]==-1){printf("Too few inputs, please press more buttons\n");}//iterates if not enough numbers have been pressed
        if(pin_nums[0]!=-1){//iterates if 4 or more keys have been pressed
        printf("Pin entered [%d][%d][%d][%d]\n",pin_nums[0],pin_nums[1],pin_nums[2],pin_nums[3]);
        printf("Please enter a 4 digit pin\n");
        pin_nums[0]=-1;//resets the values stored in the array
        pin_nums[1]=-1;
        pin_nums[2]=-1;
        pin_nums[3]=-1;}
        }}
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
