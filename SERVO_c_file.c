/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 04/19/21
* Project: Final Project
* File: SERVO_c_file.c
* Description: This library contains the function definitions that responsible for
* opening and closing the door.
**************************************************************************************/
#include "SERVO_h_file.h"
#include "msp.h"


/************************************************************************
* Function: door_open
* Inputs: None
* Outputs: None.
* Description: this function is responseable for turning on the green LED,
* setting the PWM period of the servo placing it in the open position, and
* turning off the red LED
************************************************************************/
void door_open(void){

    P2->OUT&=~BIT0;//turns off red LED

    P5->SEL0|=BIT6;//sets up P5.6 as in/out
    P5->SEL1&=~BIT6;
    P5->DIR|=BIT6;//sets P5.6 as output

    P2->SEL0 &=~ BIT1;//sets up the led on P2.1
    P2->SEL1 &=~ BIT1;
    P2->DIR |= BIT1;//sets as output

    TIMER_A2->CCR[0]=60000-1;//sets the period of timer A
    TIMER_A2->CCTL[1]=0xE0;
    TIMER_A2->CTL=0x0214;
    TIMER_A2->CCR[1]=3000;//sets duty cycle

    P2->OUT|=BIT1;//turns on green LED

}

/************************************************************************
* Function: door_close
* Inputs: None
* Outputs: None.
* Description: this function is responseable for turning on the red LED, setting
* the PWM duty cycle which places the servo in the closed position, and turning
* off the green LED
************************************************************************/
void door_close(void){

    P2->OUT&=~BIT1;//turns off green LED

    P5->SEL0|=BIT6;//sets up P5.6 as in/out
    P5->SEL1&=~BIT6;
    P5->DIR|=BIT6;//sets P5.6 as output

    P2->SEL0 &=~ BIT0;//sets up the led on P2.0
    P2->SEL1 &=~ BIT0;
    P2->DIR |= BIT0;//sets as output

    TIMER_A2->CCR[0]=60000-1;//sets the period of timer A
    TIMER_A2->CCTL[1]=0xE0;
    TIMER_A2->CTL=0x0214;
    TIMER_A2->CCR[1]=6500;//sets duty cycle

    P2->OUT|=BIT0;//turns on red LED
}
