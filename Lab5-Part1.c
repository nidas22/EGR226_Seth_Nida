/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 02/25/21
* Project: Lab 5
* File: Lab5-Part1.c
* Description: This program is designed to cycle through the three different LEDs one
* button press at a time. with the program illuminating a LED until the button is pressed
* again and then the next LED in line is illuminated.
**************************************************************************************/

#include "msp.h"

void red(int);//function to set the led to red
void green(int);//function to set the led to green
void yellow(int);//function to set the led to blue

void SysTick_delay(uint16_t delay);

void main(void)
{
    uint16_t tim=90;

P3->SEL0&=~BIT6;
P3->SEL1&=~BIT6;
P3->DIR&=~BIT6;
P3->REN|=BIT6;
P3->OUT|=BIT6;

P3->SEL0&=~BIT7;
P3->SEL1&=~BIT7;
P3->DIR&=~BIT7;
P3->REN|=BIT7;
P3->OUT|=BIT7;

P2->SEL0&=~BIT5;
P2->SEL1&=~BIT5;
P2->DIR|=BIT5;
P2->REN|=BIT5;
P2->OUT&=~BIT5;

P2->SEL0&=~BIT6;
P2->SEL1&=~BIT6;
P2->DIR|=BIT6;
P2->REN|=BIT6;
P2->OUT&=~BIT6;

P2->SEL0&=~BIT7;
P2->SEL1&=~BIT7;
P2->DIR|=BIT7;
P2->REN|=BIT7;
P2->OUT&=~BIT7;

int var=0;

while(1){//continous looping function
    if(debounce_push()){//function call which detects if the button is pushed and handles switch bounce
        SysTick_delay(tim);//a short delay that ensures the button has been released before checking if the button is no longer pressed
        if(debounce_release()){//function call which detects if the button has been released and handles switch bounce

        var++;
        if(var>3){var=1;}//keeps the variable var within the proper range

        if(var==1){//will call red() if var is 1
        green(var);}

        if(var==2){//will call green() if var is 2
        yellow(var);}

        if(var==3){//will call blue() if var is 3
        red(var);}

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
    uint16_t tim1=15;
    int pin_Value = 0; //initialize variable as low
    if ((P3IN & 0x40) == 0x00) //check of button pushed
    { SysTick_delay(tim1); //pause for 10 m-sec for switch bounce
    if ((P3IN & 0x40) == 0x00) //check of button is still pushed
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
    uint16_t tim1=15;
        int pin_Value = 0; //initialize variable as low
        if ((P3IN & 0x40) == 0x40) //check of button released
        { SysTick_delay(tim1); //pause for 10 m-sec for switch bounce
        if ((P3IN & 0x40) == 0x40) //check of button is still released
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
    P2->OUT &= ~BIT6;//turns off led if previously blue
    P2->SEL1&=~BIT5;//sets up simple I/O
    P2->SEL0&=~BIT5;
    P2->DIR|=BIT5;//sets led as out put
    P2->OUT|=BIT5;//turns led red
}

/************************************************************************
* Function: green
* Inputs: int i
* Outputs: turns the Led green and returns the value of i to main.
* Description: this function is used to change the color of the Led.
************************************************************************/
void green(int i){
    P2->OUT &= ~BIT5;//turns off led if previously red
    P2->SEL1&=~BIT7;//sets up simple I/O
    P2->SEL0&=~BIT7;
    P2->DIR|=BIT7;//sets led as output
    P2->OUT|=BIT7;//turns led green
}

/************************************************************************
* Function: blue
* Inputs: int i
* Outputs: turns the Led yellow and returns the value of i to main.
* Description: this function is used to change the color of the Led.
************************************************************************/
void yellow(int i){
    P2->OUT &= ~BIT7;//turns off led if previously green
    P2->SEL1&=~BIT6;//sets up simple I/O
    P2->SEL0&=~BIT6;
    P2->DIR|=BIT6;//sets led as output
    P2->OUT|=BIT6;//turns led blue
}

/************************************************************************
* Function: SysTick_delay
* Inputs: uint16_t delay
* Outputs: none
* Description: this function is used to delay the program by whatever
* number of milliseconds that is passed to it.
************************************************************************/
void SysTick_delay(uint16_t delay){
int i;
SysTick->LOAD=3000-1;
SysTick->VAL =0;
SysTick->CTRL=0x5;
for(i=0;i<delay;i++){
    while((SysTick->CTRL & 0x10000)==0){}
}
SysTick->CTRL=0;
}
