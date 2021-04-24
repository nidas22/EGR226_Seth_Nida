/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 04/19/21
* Project: Final Project
* File: Final-Project
* Description: This program operates a system of motors, lights, and buttons all controlled
* via a keypad and an LCD screen. A state machine in main is responseable for the seperate
* options available to the user for interaction. This program is capable of powering
* a DC motor in a range of speeds as well as opening a closing a "door" by controlling
* a servo, additionally a tricolor LED can be controlled in a range of colors and brightnesses.
**************************************************************************************/
#include "msp.h"
#include <stdio.h>
#include <stdlib.h>


#define main_menu 0;
#define door 1;
#define motor 2;
#define lights 3;

volatile    int result;
volatile    float volts;



volatile int x1=1,x2=0,x3=0,x4=0,x5=0,old_blue,old_red,old_green,lit;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;// stop watchdog timer
    motor_init();//function to intialize motor connections and PWM
	SysTick_init();//function used to initialize the Systick timer
	LCD_init();//call the LCD initialization function
    keypad_init();//function used to initialize all pins connected to the keypad
    LED_init();//function used to initialize the pins and PWM connected to the tricolor LED
    buttons();//initializes the pins connected to the e-stop button and led on/off button
    door_close();//closes the door initially in case previously open
    adcsetup();//function used to set up the ADC
    brite_set();//function used to calculate and set the brighness of the LCD via PWM

    TIMER32_2->CONTROL = 0b11100011;//timer32 initialized

    NVIC_EnableIRQ( T32_INT2_IRQn );//timer 32 interrupt enabled on reload

    NVIC_EnableIRQ(PORT3_IRQn); // Enable Port 3 interrupt on the NVIC
    __enable_irq( ); // Enable global interrupt

    TIMER32_2->LOAD = 1500000;//timer 32 load value set to 1/2 second, used for ADC calculation frequency

	int state=main_menu;//initial state


	while(1){//wile loop to contain state machine
	    switch(state){

	    case 0://main menu
	        if(x1==1){
	        display_main();}//function used to display main menu and accept input for next state
	        state=1;
	        break;
	    case 1://door
	        if(x2==1){
	        display_door();}//function used to display the door menu and allow user to open or close the door
	        state=2;
	        break;
	    case 2://motor
	        if(x2==2){
	        display_motor();}//function used to display the motor menu and allow user to set motor speed
	        state=3;
	        break;
	    case 3://lights
	        if(x2==3){
	        display_lights();}//function used to display lights menu and allow user to set individual LED brightness
	        state=0;
	        break;
	    }
	}

}

/************************************************************************
* Function: T32_INT2_IRQHandler
* Inputs: None
* Outputs: None.
* Description: timer 32 interrupt handler. this function is used to handle
* timer 32 interrupts and executes every half second. This interupt handler
* is responseable for preforming an analog to digital conversion used
* to determine the brightness of the LCD by driving a PWM proportional to
* the analog signal determined by a potentiometer.
************************************************************************/
void T32_INT2_IRQHandler (void) //Interrupt Handler for Timer 2.
{
TIMER32_2->INTCLR = 1; //Clear interrupt flag so it does not interrupt again immediately.
ADC14->CTL0 |=1; //start a conversion
while(!ADC14->IFGR0); // wait until conversion complete
result = ADC14->MEM[5]; // immediately store value in a variable
volts=result;//swaps from int to float
volts=volts/4965;//converts to a range of 0 to 3.3v
volts=volts/3.3;//converts to a decimal up to 1, i.e. a percent
TIMER_A2->CCR[2]=59000*volts;//sets PWM duty cycle that controls LCD brightness
TIMER32_2->LOAD = 1500000 ; //Set to a countdown of 1/2 second on 3 MHz clock
}

/************************************************************************
* Function: display_main
* Inputs: None
* Outputs: None.
* Description: This function clears the LCD and display the main menu, from
* which the user can chose between three sub menus door, motor, and lights.
* once the user presses a button corrresponding to a sub menu a variable
* x2 is set to the corresponding number and this function ends returning to the
* state machine in main.
************************************************************************/
void display_main(void){
    int i,key;
    char one[10]="MAIN MENU ";
    char two[9]="(1) DOOR ";
    char three[10]="(2) MOTOR ";
    char four[11]="(3) LIGHTS ";

            commandWrite(0x01);//clears the LCD screen

            commandWrite(0x84);//sets cursor on the first line
            for(i=0;i<9;i++){
                dataWrite(one[i]);//prints MAIN MENU
            }

            commandWrite(0xc1);//sets cursor on the second line
            for(i=0;i<8;i++){
                dataWrite(two[i]);//prints (1) DOOR
            }

            commandWrite(0x91);//sets cursor on the third line
            for(i=0;i<9;i++){
                dataWrite(three[i]);//prints (2) MOTOR
            }

            commandWrite(0xD1);//sets cursor on the fourth line
            for(i=0;i<10;i++){
                dataWrite(four[i]);//prints (3) LIGHTS
            }

            while(x2==0){//while loop that waits until a proper key is pressed and a sub menu is selected
                key=keypad_getkey();//gets key input from keypad
                if(key==1){x2=1;}//door menu
                if(key==2){x2=2;}//motor menu
                if(key==3){x2=3;}//lights menu
            }

x1=0;//disables the main menu in the state machine
}

/************************************************************************
* Function: display_door
* Inputs: None
* Outputs: None.
* Description: this function is responseable for displaying the door sub menu.
* once activated it will clear the screen and print the instruction to
* open or close the door by informing the user which button should be pressed
* to achieve the desired result.
************************************************************************/
void display_door(void){
    int j,key;
    char one1[10]="DOOR MENU ";
    char two1[9]="(1) OPEN ";
    char three1[10]="(2) CLOSE ";

    commandWrite(0x01);//clears screen

    commandWrite(0x84);//sets cursor on the first line
    for(j=0;j<9;j++){
        dataWrite(one1[j]);//prints DOOR MENU
    }

    commandWrite(0xc4);//sets cursor on second line
    for(j=0;j<9;j++){
        dataWrite(two1[j]);//prints (1) OPEN
    }

    commandWrite(0x94);//sets cursor on thrid line
    for(j=0;j<9;j++){
        dataWrite(three1[j]);//prints (2) CLOSE
    }

    while(x3==0){//while loop to wait for keypad input
        key=keypad_getkey();
        if(key==1){//if a 1 is pressed the door_open function is called and while loop ends
        door_open();
        x3=1;}
        if(key==2){//if a 2 is pressed the door_close function is called and while loop ends
        door_close();
        x3=1;}
    }

    x3=0;//resets the while loop above for next time
    x2=0;//resets sub menu decision
    x1=1;//sets main menu to display again once the state machine is returned to
}

/************************************************************************
* Function: brite_set
* Inputs: None
* Outputs: None.
* Description: This function is responsible for initializing the PWM
* of the pin used to control the brightness of the LCD screen.
************************************************************************/
void brite_set(void){
    P5->SEL0|=BIT7;//sets up P5.7 as in/out
    P5->SEL1&=~BIT7;
    P5->DIR|=BIT7;//sets P5.7 as output

    TIMER_A2->CCR[0]=60000-1;//sets the period of timer A
    TIMER_A2->CCTL[2]=0xE0;
    TIMER_A2->CTL=0x0214;
}

/************************************************************************
* Function: display_motor
* Inputs: None
* Outputs: None.
* Description: This function is responsible for clearing the screen and
* displaying to the user the motor control menu. Once activated the motor
* menu will promt the user to enter a number 0-9 to be used to determine the
* motor speed.
************************************************************************/
void display_motor(void){
    int k,key=0,speed;
    char one2[10]="MOTOR MENU ";
    char two2[15]="ENTER A VALUE ";
    char three2[5]="(0-9) ";


    commandWrite(0x01);//clears the screen

       commandWrite(0x83);//sets cursor on the first line
       for(k=0;k<10;k++){
       dataWrite(one2[k]);}//prints MOTOR MENU

       commandWrite(0xc1);//sets cursor on the second line
       for(k=0;k<13;k++){
       dataWrite(two2[k]);}//prints ENTER A VALUE

       commandWrite(0x95);//sets cursor on the third line
       for(k=0;k<5;k++){
       dataWrite(three2[k]);}//prints (0-9)

       while(x4==0){
           key=keypad_getkey();//function call to get keypad input from user
           if(key){
           if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
               TIMER_A1->CCR[1]=3500*key;//sets duty cycle as a multiple of the slightly less than 1/9th of the period
                   x4=1;}//sets the on time of the PWM
           if(key==10){}//used to capture * input
           if(key==12){}//used to capture # input
           if(key==11){//if 0 is pressed
               key=0;
               TIMER_A1->CCR[1]=3500*key;//sets duty cycle to 0
                   x4=1;}
           }
       }
       x4=0;//resets while loop above
       x2=0;//resets sub menu decision
       x1=1;//sets main menu to print again
}

/************************************************************************
* Function: LED_init
* Inputs: None
* Outputs: None.
* Description: This function is responseable for initializing all pins
* connected to the tri-color LED for PWM signal generation. All pins connected
* to the LED run the same period and can each run an independant duty
* cycle allowing for individual brightness control.
************************************************************************/
void LED_init(void){
    P2->SEL0|=BIT5;//sets up P2.5 as in/out
    P2->SEL1&=~BIT5;
    P2->DIR|=BIT5;//sets P2.5 as output

    TIMER_A0->CCR[0]=3000-1;//sets the period of timer A0
    TIMER_A0->CCTL[2]=0xE0;
    TIMER_A0->CTL=0x0214;
    TIMER_A0->CCR[2]=0;//duty cycle initially 0

    P2->SEL0|=BIT6;//sets up P2.6 as in/out
    P2->SEL1&=~BIT6;
    P2->DIR|=BIT6;//sets P2.6 as output

    TIMER_A0->CCR[0]=3000-1;//sets the period of timer A0
    TIMER_A0->CCTL[3]=0xE0;
    TIMER_A0->CTL=0x0214;
    TIMER_A0->CCR[3]=0;//duty cycle initially 0

     P2->SEL0|=BIT7;//sets up P2.7 as in/out
     P2->SEL1&=~BIT7;
     P2->DIR|=BIT7;//sets P2.7 as output

     TIMER_A0->CCR[0]=3000-1;//sets the period of timer A0
     TIMER_A0->CCTL[4]=0xE0;
     TIMER_A0->CTL=0x0214;
     TIMER_A0->CCR[4]=0;//duty cycle initially 0
}

/************************************************************************
* Function: display_lights
* Inputs: None
* Outputs: None.
* Description: This function is responseable for clearing the screen and
* displaying the tri color LED control menu. Once activated the user will
* be presented with the option to set the brighness of an individual LED.
************************************************************************/
void display_lights(void){
    int h,key;
    char one3[11]="LIGHT MENU ";
    char two3[9]="(1) GREEN ";
    char three3[10]="(2) RED ";
    char four3[10]="(3) BLUE ";

    x2=0;

    commandWrite(0x01);//clears the screen

       commandWrite(0x83);//places cursor on the first row
       for(h=0;h<10;h++){
       dataWrite(one3[h]);}

       commandWrite(0xc1);///places cursor on the second row
       for(h=0;h<9;h++){
       dataWrite(two3[h]);}

       commandWrite(0x91);//places the cursor n the third row
       for(h=0;h<8;h++){
       dataWrite(three3[h]);}

       commandWrite(0xD1);//places the cursor on the fourth row
       for(h=0;h<9;h++){
       dataWrite(four3[h]);}

       while(x5==0){
           key=keypad_getkey();//function call to get keypad input from user

           if(key){//if a value was returned
               if(key==1){
                   green_bright();//function to control the brightness of the green LED
                   x5=1;
               }
               if(key==2){
                   red_bright();//function to control the brightness of the red LED
                   x5=1;
               }
               if(key==3){
                   blue_bright();//function to control the brightness of the blue LED
                   x5=1;
               }
           }
       }

x1=1;//resets the state to display the main menu
x5=0;//resets the while loop above for next iteration
x2=0;//resets sub menu selection
}

/************************************************************************
* Function: green_bright
* Inputs: None
* Outputs: None.
* Description: this function is responseable for setting the duty cycle
* of the PWM signal that drives the brighness of the green LED. This is achieved
* by taking a percentage entered by the user and setting the duty cycle
* equal to that percentage
************************************************************************/

void green_bright(void){
    int key,l=10,r=11,g=10,brite,i;

    char arr[6]="GREEN ";
    char arr1[10]="ENTER A ";
    char arr2[11]="PERCENTAGE ";

    commandWrite(0x01);//clears the screen

    commandWrite(0x86);
    for(i=0;i<6;i++){
        dataWrite(arr[i]);
    }

    commandWrite(0xc4);
    for(i=0;i<8;i++){
        dataWrite(arr1[i]);
    }

    commandWrite(0x92);
    for(i=0;i<11;i++){
        dataWrite(arr2[i]);
    }


    while(l==10){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     l=key;//takes the first number entered and stores it in l
                     l=l*100;//multiplies by 100
                         }
                 if(key==11){
                     l=0;
                 }
             }
    }
    while(r==11){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     r=key;//takes the second number entered and stores it in r
                     r=r*10;//multiplies by 10
                         }
                 if(key==11){
                     r=0;
                 }
             }
    }
    while(g==10){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     g=key;//takes the third number entered and stores it in g
                     g=g;
                         }
                 if(key==11){
                     g=0;
                 }
             }
    }
    brite=l+g+r;//sums all numbers entered to total number from 0-100
    if(brite>100){brite=100;}//keeps value under 101
    old_green=brite*30;//stores previous value
    TIMER_A0->CCR[2]=30*brite;//sets duty cycle
}

/************************************************************************
* Function: red_bright
* Inputs: None
* Outputs: None.
* Description: this function is responseable for setting the duty cycle
* of the PWM signal that drives the brighness of the red LED. This is achieved
* by taking a percentage entered by the user and setting the duty cycle
* equal to that percentage
************************************************************************/

void red_bright(void){
    int key,l=10,r=11,g=10,brite,i;

    char arr[4]="RED ";
    char arr1[10]="ENTER A ";
    char arr2[11]="PERCENTAGE ";

    commandWrite(0x01);//clears screen

    commandWrite(0x85);
    for(i=0;i<4;i++){
        dataWrite(arr[i]);
    }

    commandWrite(0xc4);
    for(i=0;i<8;i++){
        dataWrite(arr1[i]);
    }

    commandWrite(0x93);
    for(i=0;i<11;i++){
        dataWrite(arr2[i]);
    }

    while(l==10){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     l=key;//stores the first value entered in l
                     l=l*100;//multiplies by 100
                         }
                 if(key==11){
                     l=0;
                 }
             }
    }
    while(r==11){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     r=key;//stores second value entered in r
                     r=r*10;//mulitpiles by 10
                         }
                 if(key==11){
                     r=0;
                 }
             }
    }
    while(g==10){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     g=key;//stores third value entered in g
                     g=g;
                         }
                 if(key==11){
                     g=0;
                 }
             }
    }
    brite=l+g+r;//sums all values entered to add up to 100
    if(brite>100){brite=100;}//ensures value is not higher than 100
    old_red=brite*30;//stores previous value of brighnetess
    TIMER_A0->CCR[3]=30*brite;//sets duty cycle
}

/************************************************************************
* Function: blue_bright
* Inputs: None
* Outputs: None.
* Description: this function is responseable for setting the duty cycle
* of the PWM signal that drives the brighness of the blue LED. This is achieved
* by taking a percentage entered by the user and setting the duty cycle
* equal to that percentage
************************************************************************/

void blue_bright(void){
    int key,l=10,r=11,g=10,brite,i;

    char arr[5]="BLUE ";
    char arr1[10]="ENTER A ";
    char arr2[11]="PERCENTAGE ";

    commandWrite(0x01);//clears the screen

    commandWrite(0x85);
    for(i=0;i<5;i++){
        dataWrite(arr[i]);
    }

    commandWrite(0xc4);
    for(i=0;i<8;i++){
        dataWrite(arr1[i]);
    }

    commandWrite(0x93);
    for(i=0;i<11;i++){
        dataWrite(arr2[i]);
    }

    while(l==10){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     l=key;//stores first number pressed in l
                     l=l*100;//multiplies by 100
                         }
                 if(key==11){
                     l=0;
                 }
             }
    }
    while(r==11){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     r=key;//stores second number pressed in r
                     r=r*10;//multiplies by 10
                         }
                 if(key==11){
                     r=0;
                 }
             }
    }
    while(g==10){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     g=key;//stores third number pressed in g
                     g=g;
                         }
                 if(key==11){
                     g=0;
                 }
             }
    }
    brite=l+g+r;//sums all numbers pressed
    if(brite>100){brite=100;}//ensures they do not exceed 100
    old_blue=brite*30;//stores previous brightness
    TIMER_A0->CCR[4]=30*brite;//sets new duty cycle
}

/************************************************************************
* Function: buttons
* Inputs: None
* Outputs: None.
* Description: This function initializes the pins connected to the
* buttons on the breadboard.
************************************************************************/
void buttons(void){
    P3->SEL0&=~0x0C;
    P3->SEL1&=~0x0C;
    P3->DIR&=~0x0C;
    P3->REN|= 0x0C; // Enable Internal resistor
    P3->OUT|= 0x0C; // Enable pull-up resistor (P1.1 output high)
    P3->IES|= 0x0C; //Set pin interrupt to trigger when it goes from high to low
    P3->IE|= 0x0C; // Enable interrupt for P1.1
    P3->IFG = 0; // Clear all P1 interrupt flags
}
/************************************************************************
* Function: PORT3_IRQHandler
* Inputs: None
* Outputs: None.
* Description: This function is responseable for handling the interrupts
* triggered by pressing the e stop button and LED killer.
************************************************************************/
void PORT3_IRQHandler(void){

if ( P3->IFG & BIT2 ) {// If P3.2 had an interrupt (going from high to low)
    TIMER_A1->CCR[1]=0;//sets the duty cycle to 0 stoping motor
}

if ( P3->IFG & BIT3 ) {// If P3.3 had an interrupt (going from high to low)
    if(lit==0){
        TIMER_A0->CCR[2]=0;//green
        TIMER_A0->CCR[3]=0;//red
        TIMER_A0->CCR[4]=0;//blue
        lit=3;
    }
    if(lit==1){
        TIMER_A0->CCR[2]=old_green;//green
        TIMER_A0->CCR[3]=old_red;//red
        TIMER_A0->CCR[4]=old_blue;//blue
        lit=2;
    }
    lit=lit-2;
}
P3->IFG &= ~BIT2; // Reset the interrupt flag
P3->IFG &= ~BIT3; // Reset the interrupt flag
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
    P5->SEL0&=~BIT5;
    P5->SEL1&=~BIT5;
    P5->DIR&=~BIT5;//sets to input

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


