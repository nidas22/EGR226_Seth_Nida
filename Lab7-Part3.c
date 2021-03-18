/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 03/17/21
* Project: Lab 7
* File: Lab7-Part3
* Description: Part three of this lab exercise is to create a program that is capable of
* printing a scrolling message. In particular this program was designed to scroll the
* words laboratory over constantly across the screen.
**************************************************************************************/
#include "msp.h"


void LCD_init(void);
void SysTick_init(void);
void delay_micro(uint32_t micro);
void delay_milli(uint16_t milli);
void PulseEnablePin(void);
void pushNibble(uint8_t nibble);
void pushByte(uint8_t byte);
void commandWrite(uint8_t command);
void dataWrite(uint8_t data);


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    SysTick_init();
    P3->OUT&=~BIT6;//set RS pin as low
    LCD_init();//calls the initialization function for the LCD

    char arr[16]={'L','A','B','O','R','A','T','O','R','Y',' ','O','V','E','R',' '};
    char arr2[32]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','L','A','B','O','R','A','T','O','R','Y',' ','O','V','E','R',' '};
    int i=0;
    int j=0;
    int k=0;

    while(1){
        if(j==0){
        for(k=0;k<16;k++){//for loop to print the message to the screen while scrolling it off the left edge
            commandWrite(0x01);
            commandWrite(0x80);
            for(i=k;i<16;i++){
            dataWrite(arr[i]);
            }
        delay_milli(500);}
        j=1;}

        if(j==1){
        for(k=0;k<16;k++){//for loop to print the message to the screen while scrolling onto the screen from the right edge
            commandWrite(0x01);
            commandWrite(0x80);
            for(i=k;i<(k+16);i++){
            dataWrite(arr2[i]);
            }
        delay_milli(500);}
        j=0;}
    }

}

/************************************************************************
* Function: LCD_init
* Inputs: None
* Outputs: None.
* Description: This function intializes the LCD with the pins connected
* to it so that future commands will be able to comunicate with the
* LCD.
************************************************************************/
void LCD_init(void){
    P4->DIR=0xF0;//sets all DBX pins as output
    P3->DIR|=BIT6;//sets the RS pin as output
    P2->DIR|=BIT3;//sets the enable pin as output
    commandWrite(3);//initialization sequence
    delay_milli(100);
    commandWrite(3);
    delay_micro(200);
    commandWrite(3);
    delay_milli(100);
    commandWrite(2);
    delay_micro(100);
    commandWrite(0x28);
    delay_micro(100);
    delay_micro(100);
    commandWrite(0x0F);
    delay_micro(100);
    commandWrite(0x01);
    delay_micro(100);
    commandWrite(0x06);
    delay_milli(10);
}

/************************************************************************
* Function: PulseEnablePin
* Inputs: None
* Outputs: None.
* Description: This function is used to pulse the enable pin with 3.3V
* for 10 microseconds which allows the LCD to update values displyed on
* the screen.
************************************************************************/
void PulseEnablePin(void){
    P2OUT &=~BIT3; // make sure pulse starts out at 0V
    delay_micro(10);
    P2OUT |=BIT3;//set the enable pin high
    delay_micro(10);//delay for 10 microseconds
    P2OUT &=~BIT3;//set the enable pin low
    delay_micro(10);
}

/************************************************************************
* Function: PushNibble
* Inputs: uint8_t nibble
* Outputs: None.
* Description: This functions purpose is to push half a byte sent from
* pushByte onto the pins connected to DB4-DB7 so that the LCD will know
* what to display on each position of the LCD.
************************************************************************/
void pushNibble(uint8_t nibble){
    P4OUT &=~0xF0; // clear P4.4-P4.7
    P4OUT |= (nibble & 0x0F) << 4; // port pins P4.4 - P4.7 wired to D4 - D7
    PulseEnablePin();//call the pulse enable function
}

/************************************************************************
* Function: PushByte
* Inputs: uint8_t byte
* Outputs: None.
* Description: This functions purpose is to push half a byte at a time
* to pushNibble. The value of the byte that the function is pushing
* comes from either commandWrite or dataWrite.
************************************************************************/
void pushByte(uint8_t byte){
    uint8_t nibble;//variable that stores each half of the byte
    nibble = (byte & 0xF0) >> 4;//stores the four most significant bits into nibble
    pushNibble(nibble);//calls push nibble and sends the first half of the byte
    nibble = byte & 0x0F;//stores the four least significant bits into nibble
    pushNibble(nibble);//calls push nibble and sends the second half of the byte
    delay_micro(100);//short delay
}

/************************************************************************
* Function: commandWrite
* Inputs: uint8_t byte
* Outputs: None.
* Description: This functions purpose is to call pushByte and pass the
* value sent from main to pushByte to begin the process of printing to
* the screen.
************************************************************************/
void commandWrite(uint8_t command){
    P3->OUT&=~BIT6;//sets the RS pin low
    pushByte(command);//calls pushByte with paramenter command
}

/************************************************************************
* Function: dataWrite
* Inputs: uint8_t byte
* Outputs: None.
* Description: This functions purpose is to call pushByte and pass the
* value sent from main to pushByte to begin the process of printing to
* the screen.
************************************************************************/
void dataWrite(uint8_t data){
    P3->OUT|=BIT6;//sets the RS pin high
    pushByte(data);//calls push byte with the parameter data
    P3->OUT&=~BIT6;//sets the RS pin low
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
* Function: delay_micro
* Inputs: uint32_t micro
* Outputs: None.
* Description: This functions purpose is to dealy the program by a
* certain number of microseconds depending upon what value is passed to
* the function
************************************************************************/
void delay_micro(uint32_t micro){
    int i;
    SysTick -> LOAD = (micro*3 - 1); // delay*3
    SysTick -> VAL = 0; //clears counter
    SysTick->CTRL=0x5;
    for(i=0;i<micro;i++){
        while((SysTick->CTRL & 0x10000)==0){}
    }
    SysTick->CTRL=0;
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
