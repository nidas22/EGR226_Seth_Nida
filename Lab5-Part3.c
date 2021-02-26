/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 02/25/21
* Project: Lab 5
* File: Lab5Part3.c
* Description: This programs purpose is to cycle through the three different LEDs
* in either direction simulaneously, in other words the user can cycle in either direction
* desired by simply pressing either button.
**************************************************************************************/

#include "msp.h"

void SysTick_delay(uint16_t delay);
int red(int);//function to turn the led red
int green(int);//function to turn the led green
int yellow(int);//function to turn the led blue

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

int j;

while(1){//continous looping function
    if(debounce_push()){//function that detects if the button is pressed and handles switch bounce
        SysTick_delay(tim);//a short delay that ensures the button has been released before checking if the button is no longer pressed
        while(debounce_release()!=1)//a while loop that iterates until the button is released
       {

        if(j>3){j=1;}//keeps the variable j within the proper bounds
        if(debounce_release()!=1){//if the button is still pressed then j is set to 1
        if(j==1){
        j=green(j);}}//the function red() is called and a incremented value of j is returned

        if(j>3){j=1;}//keeps the variable j within the proper bounds
        if(debounce_release()!=1){//if the button is still pressed then j is set to 2
        if(j==2){
        j=yellow(j);}}//the function green() is called and a incremented valueof j is returned

        if(j>3){j=1;}//keeps the variable j within the proper bounds
        if(debounce_release()!=1){//if the button is still pressed then j is set to 3
        if(j==3){
        j=red(j);}}//the function blue() is called and a incremented value of j is returned
        }
    }

    if(debounce_push1()){//function that detects if the button is pressed and handles switch bounce
            SysTick_delay(tim);//a short delay that ensures the button has been released before checking if the button is no longer pressed
            while(debounce_release1()!=1)//a while loop that iterates until the button is released
           {

            if(j<1){j=3;}//keeps the variable j within the proper bounds
            if(j>3){j=1;}
            if(debounce_release1()!=1){//if the button is still pressed then j is set to 1
            if(j==1){
            j=yellow1(j);}}//the function red() is called and a incremented value of j is returned

            if(j<1){j=3;}//keeps the variable j within the proper bounds
            if(j>3){j=1;}
            if(debounce_release1()!=1){//if the button is still pressed then j is set to 2
            if(j==2){
            j=red1(j);}}//the function green() is called and a incremented valueof j is returned

            if(j<1){j=3;}//keeps the variable j within the proper bounds
            if(j>3){j=1;}
            if(debounce_release1()!=1){//if the button is still pressed then j is set to 3
            if(j==3){
            j=green1(j);}}//the function blue() is called and a incremented value of j is returned
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
* Function: debounce_push1
* Inputs: None
* Outputs: returns a value to be used in the conditional statement from
* which the function is called.
* Description: this functions job is to handle switch bounce and allow
* for the program to detect a push of a button and ignore the folowing
* bounces/vibrations of the button.
************************************************************************/
int debounce_push1(void){
    uint16_t tim1=15;
       int pin_Value = 0; //initialize variable as low
       if ((P3IN & 0x80) == 0x00) //check of button pushed
       { SysTick_delay(tim1); //pause for 10 m-sec for switch bounce
       if ((P3IN & 0x80) == 0x00) //check of button is still pushed
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
* Function: debounce_release1
* Inputs: None
* Outputs: returns a value to be used in the conditional statement from
* which the function is called.
* Description: this functions job is to handle switch bounce and allow
* for the program to detect a release of a button and ignore the folowing
* bounces/vibrations of the button.
************************************************************************/
int debounce_release1(void){
    uint16_t tim1=15;
           int pin_Value = 0; //initialize variable as low
           if ((P3IN & 0x80) == 0x80) //check of button released
           { SysTick_delay(tim1); //pause for 10 m-sec for switch bounce
           if ((P3IN & 0x80) == 0x80) //check of button is still released
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
    uint16_t tim1=1000;
    i++;//increments j
    P2->OUT &= ~BIT6;//turns off led if previously yellow
        P2->SEL1&=~BIT5;//sets up simple I/O
        P2->SEL0&=~BIT5;
        P2->DIR|=BIT5;//sets led as out put
        P2->OUT|=BIT5;//turns led red
        SysTick_delay(tim1);
    return i;//returns j
}

/************************************************************************
* Function: red1
* Inputs: int i
* Outputs: turns the Led red and returns the value of i to main.
* Description: this function is used to change the color of the Led and
* delay the program by one second while simulataneously setting up for
* the next color change.
************************************************************************/
int red1(int i){
    uint16_t tim1=1000;
    i--;//increments j
    P2->OUT &= ~BIT7;
       P2->OUT &= ~BIT6;
    P2->OUT &= ~BIT5;//turns off led if previously green
        P2->SEL1&=~BIT5;//sets up simple I/O
        P2->SEL0&=~BIT5;
        P2->DIR|=BIT5;//sets led as out put
        P2->OUT|=BIT5;//turns led red
        SysTick_delay(tim1);
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
    uint16_t tim1=1000;
    i++;//increments j
    P2->OUT &= ~BIT5;//turns off led if previously red
        P2->SEL1&=~BIT7;//sets up simple I/O
        P2->SEL0&=~BIT7;
        P2->DIR|=BIT7;//sets led as output
        P2->OUT|=BIT7;//turns led green
        SysTick_delay(tim1);
    return i;//returns j
}

/************************************************************************
* Function: green1
* Inputs: int i
* Outputs: turns the Led green and returns the value of i to main.
* Description: this function is used to change the color of the Led and
* delay the program by one second while simulataneously setting up for
* the next color change.
************************************************************************/
int green1(int i){
    uint16_t tim1=1000;
    i--;//increments j
    P2->OUT &= ~BIT7;
       P2->OUT &= ~BIT6;
    P2->OUT &= ~BIT5;//turns off led if previously yellow
        P2->SEL1&=~BIT7;//sets up simple I/O
        P2->SEL0&=~BIT7;
        P2->DIR|=BIT7;//sets led as output
        P2->OUT|=BIT7;//turns led green
        SysTick_delay(tim1);
    return i;//returns j
}

/************************************************************************
* Function: yellow
* Inputs: int i
* Outputs: turns the Led yellow and returns the value of i to main.
* Description: this function is used to change the color of the Led and
* delay the program by one second while simulataneously setting up for
* the next color change.
************************************************************************/
int yellow(int i){
    uint16_t tim1=1000;
    i++;//increments j
    P2->OUT &= ~BIT7;//turns off led if previously green
    P2->SEL1&=~BIT6;//sets up simple I/O
    P2->SEL0&=~BIT6;
    P2->DIR|=BIT6;//sets led as output
    P2->OUT|=BIT6;//turns led blue
    SysTick_delay(tim1);
    return i;//returns j
}

/************************************************************************
* Function: yellow1
* Inputs: int i
* Outputs: turns the Led yellow and returns the value of i to main.
* Description: this function is used to change the color of the Led and
* delay the program by one second while simulataneously setting up for
* the next color change.
************************************************************************/
int yellow1(int i){
    uint16_t tim1=1000;
    i--;//increments j
    P2->OUT &= ~BIT7;
    P2->OUT &= ~BIT6;
    P2->OUT &= ~BIT5;//turns off led if previously red
    P2->SEL1&=~BIT6;//sets up simple I/O
    P2->SEL0&=~BIT6;
    P2->DIR|=BIT6;//sets led as output
    P2->OUT|=BIT6;//turns led blue
    SysTick_delay(tim1);
    return i;//returns j
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
