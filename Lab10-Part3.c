/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 03/29/21
* Project: Lab 10
* File: Lab10-Part3.c
* Description: Part three of this lab exercise required students to create a program
* using the temperature sensor and a LCD display of which was capable of displaying the
* current ambient temperature of the room or enviornment to the LCD screen in half second
* intervals.
**************************************************************************************/
#include "msp.h"
#include <stdio.h>

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
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	int result,j,i;
	float volts;
	char arr1[12]={'T','E','M','P','E','R','A','T','U','R','E',' '};
	char arr2[6];


	P3->OUT&=~BIT6;//set RS pin low
    P5->SEL0&=~BIT5;
    P5->SEL1&=~BIT5;
    P5->DIR&=~BIT5;//sets to input

    adcsetup();
    SysTick_init();//function used to initialize the Systick timer
    LCD_init();//call the LCD initialization function


    while(1){
        commandWrite(0x01);
        ADC14->CTL0 |=1; //start a conversion
        while(!ADC14->IFGR0); // wait until conversion complete
        result = ADC14->MEM[5]; // immediately store value in a variable
        volts=result;
        volts=volts/4965;
        volts=volts*1000;
        volts=(volts-500)/10;
        j=volts*100;

        sprintf(arr2, "%d", j);
        arr2[4]='.';
        commandWrite(0x82);//allows the word to be center alligned
        for(i=0;i<12;i++){
            dataWrite(arr1[i]);
        }
        commandWrite(0xc4);//places the cusor on the secon line
        dataWrite(arr2[0]);//prints the temp
        dataWrite(arr2[1]);
        dataWrite(arr2[4]);
        dataWrite(arr2[2]);
        dataWrite(arr2[3]);
        dataWrite(0xDF);//prints the degree symbol
        delay_milli(500);//half second delay
    }
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
