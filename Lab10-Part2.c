/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 03/29/21
* Project: Lab 10
* File: Lab10-Part2.c
* Description: Part two of this lab exercise required students to modify the circuit from
* part one so that instead of a potentiometer the ADC wuld be cnverting the voltage output
* from a temperature sensor.
**************************************************************************************/
#include "msp.h"
#include <stdio.h>


    volatile int result;
    volatile float volts;



void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    P5->SEL0&=~BIT5;
    P5->SEL1&=~BIT5;
    P5->DIR&=~BIT5;//sets to input

    adcsetup();
	SysTick_Init();//function used to initialize the Systick timer


	while(1){}
}

/************************************************************************
* Function: adcsetup
* Inputs: None
* Outputs: None.
* Description: This function initializes the analog to digital converter
* connected to P5.5
************************************************************************/
void adcsetup(void)
{
ADC14->CTL0 = 0x00000010; //power on and disabled during configuration
ADC14->CTL0 |= 0x04D80300; // S/H pulse mode, MCLCK, 32 sample clocks,sw trigger, /4 clock divide
ADC14->CTL1 = 0x00000030; // 14-bit resolution
ADC14->MCTL[5] = 0; // A0 input, single ended, vref=avcc
P5->SEL1 |= 0x20; // configure pin 5.5 for AO
P5->SEL0 |= 0x20;
ADC14->CTL1 |= 0x00050000; //start converting at mem reg 5
ADC14->CTL0 |= 2; //enable ADC after configuration
}

/************************************************************************
* Function: SysTick_Init
* Inputs: None
* Outputs: None.
* Description: This function initializes the systick timer to have reload
* value equal to one half second and have interrupts that trigger on the reload
* of the timer.
************************************************************************/
void SysTick_Init (void) { //initialization of systic timer
SysTick -> CTRL = 0; // disable SysTick During set up
SysTick -> LOAD = 1500000; // reload value for 1/2s interrupts
SysTick -> VAL = 0; // any write to current clears it
SysTick -> CTRL = 0x00000007; // enable systic, 3MHz, Interrupts
}

/************************************************************************
* Function: SysTick_Handler
* Inputs: None
* Outputs: None.
* Description: function used to handle systick interrupts, in this
* program the systick handler is used to preform ADC calculations.
************************************************************************/
void SysTick_Handler (void) {
    ADC14->CTL0 |=1; //start a conversion
    while(!ADC14->IFGR0); // wait until conversion complete
    result = ADC14->MEM[5]; // immediately store value in a variable
    volts=result;
    volts=volts/4965;
    volts=volts*1000;
    volts=(volts-500)/10;
    printf("Temperature is: %g",volts);
}
