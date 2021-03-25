/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 03/25/21
* Project: Lab 9
* File: Lab9-Part1.c
* Description: Part one of this lab exercise required students to control the speed
* of a DC motor using a PWM signal that has a duty cycle controlled by the press of
* three seperate buttons of which trigger interrupts. when a button is pressed depending
* on which button it will either increment or decrement the speed of the motor by 10
* percent or stop the motor entirely. This is done inside interrupts utilized from
* Timer A.
**************************************************************************************/
#include "msp.h"

volatile int width=0;//controls the duty cycle

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	TIMER_A0->CCR[0]=50000-1;//initializes timer A period
	TIMER_A0->CCTL[4]=0x40;
	TIMER_A0->CTL=0x0234;

	buttons();//function used to initialize the pins connected to the buttons
	motor();//functions used to initialize the pins connected to the motor

	NVIC_EnableIRQ(PORT3_IRQn); // Enable Port 3 interrupt on the NVIC
	__enable_irq( ); // Enable global interrupt

	while(1){}//empty while loop to keep the MSP busy
}

/************************************************************************
* Function: buttons
* Inputs: None
* Outputs: None.
* Description: This function initializes the pins connected to the
* buttons on the breadboard.
************************************************************************/
void buttons(void){
    P3->SEL0&=~0xE0;
    P3->SEL1&=~0xE0;
    P3->DIR&=~0xE0;
    P3->REN|= 0xE0; // Enable Internal resistor
    P3->OUT|= 0xE0; // Enable pull-up resistor (P1.1 output high)
    P3->IES|= 0xE0; //Set pin interrupt to trigger when it goes from high to low
    P3->IE|= 0xE0; // Enable interrupt for P1.1
    P3->IFG = 0; // Clear all P1 interrupt flags
}

/************************************************************************
* Function: motor
* Inputs: None
* Outputs: None.
* Description: This function initializes the pins connected to the
* DC motor.
************************************************************************/
void motor(void){
     P2->SEL0|=BIT7;//sets up P2.7 as in/out
     P2->SEL1&=~BIT7;
     P2->DIR|=BIT7;//sets P2.7 as output
}

/************************************************************************
* Function: PORT3_IRQHandler
* Inputs: None
* Outputs: None.
* Description: This function handles the interrupts that were triggered by
* the presses of the buttons. Inside this function once interrupts are
* triggered the duty cycle of the PWM signal is adjusted and lastly all
* interrupt flags are reset.
************************************************************************/
void PORT3_IRQHandler(void){

if ( P3->IFG & BIT5 ) {// If P3.5 had an interrupt (going from high to low)
    width=width+4999;//increases the duty cycle by 10 percent
    if(width>49999){width=49999;}//ensurse a value greater than 100 percent cant be sent to the motor
    TIMER_A0->CCR[4]=width;//sets the duty cycle
}

if ( P3->IFG & BIT6 ) {// If P3.6 had an interrupt (going from high to low)
    width=width-4999;//decreases the duty cycle by 10 percent
    if(width<0){width=0;}//ensures a value less than 0 percent cant be sent to the motor
    TIMER_A0->CCR[4]=width;//sets the duty cycle
}

if ( P3->IFG & BIT7 ) {// If P3.7 had an interrupt (going from high to low)
    width=0;//sets the duty cycle to 0
    TIMER_A0->CCR[4]=width;//sets the duty cycle
}

P3->IFG &= ~BIT5; // Reset the interrupt flag
P3->IFG &= ~BIT6; // Reset the interrupt flag
P3->IFG &= ~BIT7; // Reset the interrupt flag
}
