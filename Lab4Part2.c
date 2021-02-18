/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 02/18/21
* Project: Lab 4
* File: Lab4Part2.c
* Description: This programs pupose is to control an onboard led on the MSP432 launchpad.
* accpeting user input via a push button the program cycles through red, green, and blue
* while the push button is held down spending one second displaying each color.
**************************************************************************************/

#include "msp.h"


int red(int);//function to turn the led red
int green(int);//function to turn the led green
int blue(int);//function to turn the led blue

void main(void)
{
P1->SEL1&=~2;//sets the button as simple I/O
P1->SEL0&=~2;
P1->DIR&=~2;//sets the button to input
P1->REN |=2;//enables the internal pull up resistor
P1->OUT |=2;//sets the led to output

int j=1;

while(1){//continous looping function
    if(debounce_push()){//function that detects if the button is pressed and handles switch bounce
        __delay_cycles(5000);//a short delay
        while(debounce_release()!=1)//a while loop that iterates until the button is released
       {

        if(j>3){j=1;}//keeps the variable j within the proper bounds
        if(debounce_release()!=1){//if the button is still pressed then j is set to 1
        if(j==1){
        j=red(j);}}//the function red() is called and a incremented value of j is returned

        if(j>3){j=1;}//keeps the variable j within the proper bounds
        if(debounce_release()!=1){//if the button is still pressed then j is set to 2
        if(j==2){
        j=green(j);}}//the function green() is called and a incremented valueof j is returned

        if(j>3){j=1;}//keeps the variable j within the proper bounds
        if(debounce_release()!=1){//if the button is still pressed then j is set to 3
        if(j==3){
        j=blue(j);}}//the function blue() is called and a incremented value of j is returned
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
    { __delay_cycles(5000); //pause for 10 m-sec for switch bounce
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
        if ((P1IN & 0x02) == 0x02) //check of button pushed
        { __delay_cycles(5000); //pause for 10 m-sec for switch bounce
        if ((P1IN & 0x02) == 0x02) //check of button is still pushed
        pin_Value = 1;
        }
        return pin_Value;
}

/************************************************************************
* Function: red
* Inputs: int i
* Outputs: turns the Led red and returns the value of i to main.
* Description: this function is used to change the color of the Led and
* delay the program by one second while simulataneously setting up for
* the next color change.
************************************************************************/
int red(int i){
    i++;//increments j
    P2->OUT &= ~4;//turns the led off if blue is still shown
    P2->SEL1&=~1;//sets up simple I/O
    P2->SEL0&=~1;
    P2->DIR|=1;//sets the led to output
    P2->OUT|=1;//turns the led red
    __delay_cycles(3000000);//delay for 1 second
    return i;//returns j
}

/************************************************************************
* Function: green
* Inputs: int i
* Outputs: turns the Led green and returns the value of i to main.
* Description: this function is used to change the color of the Led and
* delay the program by one second while simulataneously setting up for
* the next color change.
************************************************************************/
int green(int i){
    i++;//increments j
    P2->OUT &= ~1;//turns the led off if red is still shown
    P2->SEL1&=~2;//sets up simple I/O
    P2->SEL0&=~2;
    P2->DIR|=2;//sets the led to output
    P2->OUT|=2;//turns the led green
    __delay_cycles(3000000);//delay for 1 second
    return i;//returns j
}

/************************************************************************
* Function: blue
* Inputs: int i
* Outputs: turns the Led blue and returns the value of i to main.
* Description: this function is used to change the color of the Led and
* delay the program by one second while simulataneously setting up for
* the next color change.
************************************************************************/
int blue(int i){
    i++;//increments j
    P2->OUT &= ~2;//turns the led off if green is still shown
    P2->SEL1&=~4;//sets up simple I/O
    P2->SEL0&=~4;
    P2->DIR|=4;//sets the led to output
    P2->OUT|=4;//turns the led green
    __delay_cycles(3000000);//delay for 1 second
    return i;//returns j
}
