/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 02/18/21
* Project: Lab 4
* File: Lab4Part1.c
* Description: This programs pupose is to control an onboard led on the MSP432 launchpad.
* accpeting user input via a push button the program cycles through red, green, and blue
* with each push of the button. This program also deals with switch bounce and is capable
* of ignoring it.
**************************************************************************************/

#include "msp.h"

void red(int);//function to set the led to red
void green(int);//function to set the led to green
void blue(int);//function to set theled to blue

void main(void)
{
P1->SEL1&=~2;//sets the button to I/O
P1->SEL0&=~2;
P1->DIR&=~2;//set the button to input
P1->REN |=2;//sets up the internal pull up resistor
P1->OUT |=2;//sets the led to output

int var=0;

while(1){//continous looping function
    if(debounce_push()){//function call which detects if the button is pushed and handles switch bounce
        __delay_cycles(5000);//a short delay that ensures the button has been released before checking if the button is no longer pressed
        if(debounce_release()){//function call which detects if the button has been released and handles switch bounce

        var++;
        if(var>3){var=1;}//keeps the variable var within the proper range

        if(var==1){//will call red() if var is 1
        red(var);}

        if(var==2){//will call green() if var is 2
        green(var);}

        if(var==3){//will call blue() if var is 3
        blue(var);}

    }
}
}
}
/************************************************************************
* Function: debounce_push
* Inputs: None
* Outputs: returns a value to be used in the conditional statement from
* which the function is called.
* Description: this functions job is to handle switch bounce and allow
* for the program to detect a push of a button and ignore the folowing
* bounces/vibrations of the button.
************************************************************************/
int debounce_push(void){
    int pin_Value = 0; //initialize variable as low
    if ((P1IN & 0x02) == 0x00) //check of button pushed
    { __delay_cycles(500); //pause for 10 m-sec for switch bounce
    if ((P1IN & 0x02) == 0x00) //check of button is still pushed
    pin_Value = 1;
    }
    return pin_Value;
}
/************************************************************************
* Function: debounce_release
* Inputs: None
* Outputs: returns a value to be used in the conditional statement from
* which the function is called.
* Description: this functions job is to handle switch bounce and allow
* for the program to detect a release of a button and ignore the folowing
* bounces/vibrations of the button.
************************************************************************/
int debounce_release(void){
        int pin_Value = 0; //initialize variable as low
        if ((P1IN & 0x02) == 0x02) //check of button released
        { __delay_cycles(500); //pause for 10 m-sec for switch bounce
        if ((P1IN & 0x02) == 0x02) //check of button is still released
        pin_Value = 1;
        }
        return pin_Value;
}

/************************************************************************
* Function: red
* Inputs: int i
* Outputs: turns the Led red and returns the value of i to main.
* Description: this function is used to change the color of the Led.
************************************************************************/
void red(int i){
    P2->OUT &= ~4;//turns off led if previously blue
    P2->SEL1&=~1;//sets up simple I/O
    P2->SEL0&=~1;
    P2->DIR|=1;//sets led as out put
    P2->OUT|=1;//turns led red
}

/************************************************************************
* Function: green
* Inputs: int i
* Outputs: turns the Led green and returns the value of i to main.
* Description: this function is used to change the color of the Led.
************************************************************************/
void green(int i){
    P2->OUT &= ~1;//turns off led if previously red
    P2->SEL1&=~2;//sets up simple I/O
    P2->SEL0&=~2;
    P2->DIR|=2;//sets led as output
    P2->OUT|=2;//turns led green
}

/************************************************************************
* Function: blue
* Inputs: int i
* Outputs: turns the Led blue and returns the value of i to main.
* Description: this function is used to change the color of the Led.
************************************************************************/
void blue(int i){
    P2->OUT &= ~2;//turns off led if previously green
    P2->SEL1&=~4;//sets up simple I/O
    P2->SEL0&=~4;
    P2->DIR|=4;//sets led as output
    P2->OUT|=4;//turns led blue
}
