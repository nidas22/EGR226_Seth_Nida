/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 03/18/21
* Project: Lab 8
* File: Lab8-Part2.c
* Description: Part two of this lab achieves essentialy the same thing as part one
* by controlling the speed of a DC motor using a PWM signal. The difference being that
* in part two the PWM signal is generated using Timer A.
**************************************************************************************/
#include "msp.h"

void main(void)
{

int pulse=0;

P2->SEL0|=BIT7;//sets up P2.7 as in/out
P2->SEL1&=~BIT7;
P2->DIR|=BIT7;//sets P2.7 as output

TIMER_A0->CCR[0]=50000-1;//initializes timer A period
TIMER_A0->CCTL[4]=0x40;
TIMER_A0->CTL=0x0234;

while(1){
    TIMER_A0->CCR[4]=5555*pulse;//determines "on" time of the PWM
}

}
