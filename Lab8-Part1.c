/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 03/18/21
* Project: Lab 8
* File: Lab8-Part1.c
* Description: Part one of this lab requires students to create a psuedo PWM code
* that runs off of the systick timer to control the speed of a dc motor and the duty
* cycle can be modified while the code iterates.
**************************************************************************************/
#include "msp.h"



void main(void)
{

	SysTick_init();//function call to intialize the systick timer

	P4->SEL0&=~BIT0;//sets up P4.0 to in/out
    P4->SEL1&=~BIT0;
    P4->DIR|=BIT0;//sets P4.0 as an output

    int PulseWidth=20;//initializes the "on" time

    while(1){
        P4->OUT|=BIT0;//sets P4.0 HIGH
        delay_milli(PulseWidth);//delays with the motor on for the duration of the on time
        P4->OUT&=~BIT0;//sets P4.0 LOW
        delay_milli(100-PulseWidth);//delays with the motor off for the duration of the off time
    }

}

/************************************************************************
* Function: SysTick_init
* Inputs: None
* Outputs: None.
* Description: This functions purpose is to initialize the systick timer.
************************************************************************/
void SysTick_init(void){
    SysTick -> CTRL = 0; // disable SysTick During step
    SysTick -> LOAD = 0x00FFFFFF; // max reload value
    SysTick -> VAL = 0; // any write to current clears it
    SysTick -> CTRL = 0x00000005; // enable systic, 3MHz, No Interrupts
}


/************************************************************************
* Function: delay_milli
* Inputs: uint32_t milli
* Outputs: None.
* Description: This functions purpose is to dealy the program by a
* certain number of milliseconds depending upon what value is passed to
* the function
************************************************************************/
void delay_milli(uint16_t milli){
    int i;
    SysTick->LOAD=3000-1;
    SysTick->VAL =0;
    SysTick->CTRL=0x5;
    for(i=0;i<milli;i++){
        while((SysTick->CTRL & 0x10000)==0){}
    }
    SysTick->CTRL=0;
}

