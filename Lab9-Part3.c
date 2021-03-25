/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 03/25/21
* Project: Lab 9
* File: Lab9-Part3.c
* Description: Part three of this lab activity required students to create a program that
* would essentially combine certain parts of part one and two into one program. By this
* i mean the program was required to increment or decrement the number displayed on the
* seven segment LED with the press of one of two buttons.
**************************************************************************************/
#include "msp.h"

volatile int count=0;
volatile int m=180;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    SysTick_Init();
    P4->SEL0&=~0xFF;
    P4->SEL0&=~0xFF;
    P4->DIR|=0xFF;//sets P4.0-P4.7 as output

    buttons();//function used to initialize the pins connected to the buttons

    NVIC_EnableIRQ(PORT3_IRQn); // Enable Port 3 interrupt on the NVIC
    __enable_irq( ); // Enable global interrupt

    while(1){}//keeps the MSP busy
}

/************************************************************************
* Function: buttons
* Inputs: None
* Outputs: None.
* Description: This function initializes the pins connected to the
* buttons on the breadboard.
************************************************************************/
void buttons(void){
    P3->SEL0&=~0x60;
    P3->SEL1&=~0x60;
    P3->DIR&=~0x60;
    P3->REN|= 0x60; // Enable Internal resistor
    P3->OUT|= 0x60; // Enable pull-up resistor (P3.6 + P3.5 output high)
    P3->IES|= 0x60; //Set pin interrupt to trigger when it goes from high to low
    P3->IE|= 0x60; // Enable interrupt for P3.6 + P3.5
    P3->IFG = 0; // Clear all P3 interrupt flags
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

    if ( P3->IFG & BIT5 ) {
        delay_milli(m);//short delay to handle switch bounce
        P4->OUT&=~0xFF;
        count++;//increments the number displayed
        if(count>9){count=0;}
        if(count==0){P4->OUT|=0x3F;}
        if(count==1){P4->OUT|=0x06;}
        if(count==2){P4->OUT|=0x5B;}
        if(count==3){P4->OUT|=0x4F;}
        if(count==4){P4->OUT|=0x66;}
        if(count==5){P4->OUT|=0x6D;}
        if(count==6){P4->OUT|=0x7D;}
        if(count==7){P4->OUT|=0x07;}
        if(count==8){P4->OUT|=0x7F;}
        if(count==9){P4->OUT|=0x67;}
    }

    if ( P3->IFG & BIT6 ) {
        delay_milli(m);//short delay to handle switch bounce
        P4->OUT&=~0xFF;
        count--;//decrements the number displayed
        if(count<0){count=9;}
        if(count==0){P4->OUT|=0x3F;}
        if(count==1){P4->OUT|=0x06;}
        if(count==2){P4->OUT|=0x5B;}
        if(count==3){P4->OUT|=0x4F;}
        if(count==4){P4->OUT|=0x66;}
        if(count==5){P4->OUT|=0x6D;}
        if(count==6){P4->OUT|=0x7D;}
        if(count==7){P4->OUT|=0x07;}
        if(count==8){P4->OUT|=0x7F;}
        if(count==9){P4->OUT|=0x67;}
    }

P3->IFG &= ~BIT5; // Reset the interrupt flag
P3->IFG &= ~BIT6; // Reset the interrupt flag
}

/************************************************************************
* Function: SysTick_Init
* Inputs: None
* Outputs: None.
* Description: This function initializes the systick timer with no interrupts.
************************************************************************/
void SysTick_Init (void) { //initialization of systic timer
    SysTick -> CTRL = 0; // disable SysTick During step
    SysTick -> LOAD = 0x00FFFFFF; // max reload value
    SysTick -> VAL = 0; // any write to current clears it
    SysTick -> CTRL = 0x00000005; // enable systic, 3MHz, No Interrupts
}

/************************************************************************
* Function: delay_milli
* Inputs: None
* Outputs: None.
* Description: uses the systick timer to create millisecond delays
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
