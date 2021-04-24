/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 04/19/21
* Project: Final Project
* File: MOTOR_c_file.c
* Description: This library contains the motor initializatioon functions of which
* is responsible for initializing the PWM signal that controls the motor speed of the
* DC motor.
**************************************************************************************/
#include "MOTOR_h_file.h"
#include "msp.h"

/************************************************************************
* Function: motor_init
* Inputs: None
* Outputs: None.
* Description: This function is responseable for setting the PWM signal
* used to control the motor speed. This particular PWM signal is set to
* 40 Hz and intially set with a duty cycle of 0.
************************************************************************/
void motor_init(void){
    P7->SEL0|=BIT7;//sets up P7.7 as in/out
    P7->SEL1&=~BIT7;
    P7->DIR|=BIT7;//sets P7.7 as output

    TIMER_A1->CCR[0]=37500-1;//sets the period of timer A1 to 40Hz
    TIMER_A1->CCTL[1]=0xE0;
    TIMER_A1->CTL=0x0614;
    TIMER_A1->CCR[1]=0;//sets duty cycle to 0 initially

}

