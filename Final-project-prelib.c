/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 04/19/21
* Project: Final Project
* File: Final-Project
* Description:
**************************************************************************************/
#include "msp.h"
#include <stdio.h>

//LEDA for lcd brightness

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
    motor_init();
	SysTick_init();//function used to initialize the Systick timer
	LCD_init();//call the LCD initialization function
    keypad_init();
    LED_init();
    buttons();
    door_close();
    adcsetup();
    brite_set();

    TIMER32_2->CONTROL = 0b11100011;

    NVIC_EnableIRQ( T32_INT2_IRQn );

    NVIC_EnableIRQ(PORT3_IRQn); // Enable Port 3 interrupt on the NVIC
    __enable_irq( ); // Enable global interrupt

    TIMER32_2->LOAD = 1500000;

	int state=main_menu;


	while(1){
	    switch(state){

	    case 0://main menu
	        if(x1==1){
	        display_main();}
	        state=1;
	        break;
	    case 1://door
	        if(x2==1){
	        display_door();}
	        state=2;
	        break;
	    case 2://motor
	        if(x2==2){
	        display_motor();}
	        state=3;
	        break;
	    case 3://lights
	        if(x2==3){
	        display_lights();}
	        state=0;
	        break;
	    }
	}

}

void T32_INT2_IRQHandler (void) //Interrupt Handler for Timer 2.
{
TIMER32_2->INTCLR = 1; //Clear interrupt flag so it does not interrupt again immediately.
ADC14->CTL0 |=1; //start a conversion
while(!ADC14->IFGR0); // wait until conversion complete
result = ADC14->MEM[5]; // immediately store value in a variable
volts=result;
volts=volts/4965;
volts=volts/3.3;
TIMER_A2->CCR[2]=59000*volts;
TIMER32_2->LOAD = 1500000 ; //Set to a countdown of 1/2 second on 3 MHz clock
}

void display_main(void){
    int i,key;
    char one[10]="MAIN MENU ";
    char two[9]="(1) DOOR ";
    char three[10]="(2) MOTOR ";
    char four[11]="(3) LIGHTS ";

            commandWrite(0x01);

            commandWrite(0x84);
            for(i=0;i<9;i++){
                dataWrite(one[i]);
            }

            commandWrite(0xc1);
            for(i=0;i<8;i++){
                dataWrite(two[i]);
            }

            commandWrite(0x91);
            for(i=0;i<9;i++){
                dataWrite(three[i]);
            }

            commandWrite(0xD1);
            for(i=0;i<10;i++){
                dataWrite(four[i]);
            }

            while(x2==0){
                key=keypad_getkey();
                if(key==1){x2=1;}
                if(key==2){x2=2;}
                if(key==3){x2=3;}
            }

x1=0;
}

void display_door(void){
    int j,key;
    char one1[10]="DOOR MENU ";
    char two1[9]="(1) OPEN ";
    char three1[10]="(2) CLOSE ";

    commandWrite(0x01);

    commandWrite(0x84);
    for(j=0;j<9;j++){
        dataWrite(one1[j]);
    }

    commandWrite(0xc4);
    for(j=0;j<9;j++){
        dataWrite(two1[j]);
    }

    commandWrite(0x94);
    for(j=0;j<9;j++){
        dataWrite(three1[j]);
    }

    while(x3==0){
        key=keypad_getkey();
        if(key==1){
        door_open();
        x3=1;}
        if(key==2){
        door_close();
        x3=1;}
    }

    x3=0;
    x2=0;
    x1=1;
}

void door_open(void){

    P2->OUT&=~BIT0;//turns off red LED

    P5->SEL0|=BIT6;//sets up P5.6 as in/out
    P5->SEL1&=~BIT6;
    P5->DIR|=BIT6;//sets P2.7 as output

    P2->SEL0 &=~ BIT1;//sets up the led on P2.1
    P2->SEL1 &=~ BIT1;
    P2->DIR |= BIT1;//sets as output

    TIMER_A2->CCR[0]=60000-1;//sets the period of timer A
    TIMER_A2->CCTL[1]=0xE0;
    TIMER_A2->CTL=0x0214;
    TIMER_A2->CCR[1]=3000;

    P2->OUT|=BIT1;//turns on green LED

}

void door_close(void){

    P2->OUT&=~BIT1;//turns off green LED

    P5->SEL0|=BIT6;//sets up P2.7 as in/out
    P5->SEL1&=~BIT6;
    P5->DIR|=BIT6;//sets P2.7 as output

    P2->SEL0 &=~ BIT0;//sets up the led on P2.0
    P2->SEL1 &=~ BIT0;
    P2->DIR |= BIT0;//sets as output

    TIMER_A2->CCR[0]=60000-1;//sets the period of timer A
    TIMER_A2->CCTL[1]=0xE0;
    TIMER_A2->CTL=0x0214;
    TIMER_A2->CCR[1]=6500;

    P2->OUT|=BIT0;//turns on red LED
}

void brite_set(void){
    P5->SEL0|=BIT7;//sets up P2.7 as in/out
    P5->SEL1&=~BIT7;
    P5->DIR|=BIT7;//sets P2.7 as output

    TIMER_A2->CCR[0]=60000-1;//sets the period of timer A
    TIMER_A2->CCTL[2]=0xE0;
    TIMER_A2->CTL=0x0214;

}

void display_motor(void){
    int k,key=0,speed;
    char one2[10]="MOTOR MENU ";
    char two2[15]="ENTER A VALUE ";
    char three2[5]="(0-9) ";


    commandWrite(0x01);

       commandWrite(0x83);
       for(k=0;k<10;k++){
       dataWrite(one2[k]);}

       commandWrite(0xc1);
       for(k=0;k<13;k++){
       dataWrite(two2[k]);}

       commandWrite(0x95);
       for(k=0;k<5;k++){
       dataWrite(three2[k]);}

       while(x4==0){

           key=keypad_getkey();//function call to get keypad input from user

           if(key){

           if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
               TIMER_A1->CCR[1]=3500*key;
                   x4=1;}//sets the on time of the PWM

           if(key==10){}//used to capture * input

           if(key==12){}//used to capture # input

           if(key==11){
               key=0;
               TIMER_A1->CCR[1]=3500*key;
                   x4=1;}//sets the on time of the PWM
           }

       }


       x4=0;
       x2=0;
       x1=1;
}



void motor_init(void){
    P7->SEL0|=BIT7;//sets up P2.7 as in/out
    P7->SEL1&=~BIT7;
    P7->DIR|=BIT7;//sets P2.7 as output

    TIMER_A1->CCR[0]=37500-1;//sets the period of timer A1
    TIMER_A1->CCTL[1]=0xE0;
    TIMER_A1->CTL=0x0614;
    TIMER_A1->CCR[1]=0;

}

void LED_init(void){
    P2->SEL0|=BIT5;//sets up P2.7 as in/out
    P2->SEL1&=~BIT5;
    P2->DIR|=BIT5;//sets P2.7 as output

    TIMER_A0->CCR[0]=3000-1;//sets the period of timer A0
    TIMER_A0->CCTL[2]=0xE0;
    TIMER_A0->CTL=0x0214;
    TIMER_A0->CCR[2]=0;

    P2->SEL0|=BIT6;//sets up P2.7 as in/out
    P2->SEL1&=~BIT6;
    P2->DIR|=BIT6;//sets P2.7 as output

    TIMER_A0->CCR[0]=3000-1;//sets the period of timer A0
    TIMER_A0->CCTL[3]=0xE0;
    TIMER_A0->CTL=0x0214;
    TIMER_A0->CCR[3]=0;

     P2->SEL0|=BIT7;//sets up P2.7 as in/out
     P2->SEL1&=~BIT7;
     P2->DIR|=BIT7;//sets P2.7 as output

     TIMER_A0->CCR[0]=3000-1;//sets the period of timer A0
     TIMER_A0->CCTL[4]=0xE0;
     TIMER_A0->CTL=0x0214;
     TIMER_A0->CCR[4]=0;

}

void display_lights(void){
    int h,key;
    char one3[11]="LIGHT MENU ";
    char two3[9]="(1) GREEN ";
    char three3[10]="(2) RED ";
    char four3[10]="(3) BLUE ";

    x2=0;

    commandWrite(0x01);//clears the screen

       commandWrite(0x83);//places cusor on the first row
       for(h=0;h<10;h++){
       dataWrite(one3[h]);}

       commandWrite(0xc1);///places cursor on the second row
       for(h=0;h<9;h++){
       dataWrite(two3[h]);}

       commandWrite(0x91);
       for(h=0;h<8;h++){
       dataWrite(three3[h]);}

       commandWrite(0xD1);
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
x5=0;
x2=0;

}

void green_bright(void){
    int key,l=10,r=10,g=10,brite,i;

    char arr[6]="GREEN ";
    char arr1[10]="ENTER A ";
    char arr2[11]="PERCENTAGE ";

    commandWrite(0x01);

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
                     l=key;
                     l=l*100;
                         }
                 if(key==11){
                     l=0;
                 }
             }
    }
    while(r==10){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     r=key;
                     r=r*10;
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
                     g=key;
                     g=g;
                         }
                 if(key==11){
                     g=0;
                 }
             }
    }
    brite=l+g+r;
    if(brite>100){brite=100;}
    old_green=brite*30;
    TIMER_A0->CCR[2]=30*brite;


}

void red_bright(void){
    int key,l=10,r=10,g=10,brite,i;

    char arr[4]="RED ";
    char arr1[10]="ENTER A ";
    char arr2[11]="PERCENTAGE ";

    commandWrite(0x01);

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
                     l=key;
                     l=l*100;
                         }
                 if(key==11){
                     l=0;
                 }
             }
    }
    while(r==10){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     r=key;
                     r=r*10;
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
                     g=key;
                     g=g;
                         }
                 if(key==11){
                     g=0;
                 }
             }
    }
    brite=l+g+r;
    if(brite>100){brite=100;}
    old_red=brite*30;
    TIMER_A0->CCR[3]=30*brite;



}

void blue_bright(void){
    int key,l=10,r=10,g=10,brite,i;

    char arr[5]="BLUE ";
    char arr1[10]="ENTER A ";
    char arr2[11]="PERCENTAGE ";

    commandWrite(0x01);

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
                     l=key;
                     l=l*100;
                         }
                 if(key==11){
                     l=0;
                 }
             }
    }
    while(r==10){
        key=keypad_getkey();//function call to get keypad input from user

             if(key){
                 if((key!=10)&&(key!=12)&&(key!=11)){//iterates if the key pressed is 1-9
                     r=key;
                     r=r*10;
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
                     g=key;
                     g=g;
                         }
                 if(key==11){
                     g=0;
                 }
             }
    }
    brite=l+g+r;
    if(brite>100){brite=100;}
    old_blue=brite*30;
    TIMER_A0->CCR[4]=30*brite;
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

void PORT3_IRQHandler(void){

if ( P3->IFG & BIT2 ) {// If P3.5 had an interrupt (going from high to low)
    TIMER_A1->CCR[1]=0;//sets the duty cycle
}

if ( P3->IFG & BIT3 ) {// If P3.6 had an interrupt (going from high to low)
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
* Function: LCD_init
* Inputs: None
* Outputs: None.
* Description: This function intializes the LCD with the pins connected
* to it so that future commands will be able to comunicate with the
* LCD.
************************************************************************/
void LCD_init(void){
    P6->DIR=0xF0;//sets all DBX pins as output
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
    P6OUT &=~0xF0; // clear P4.4-P4.7
    P6OUT |= (nibble & 0x0F) << 4; // port pins P4.4 - P4.7 wired to D4 - D7
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

/************************************************************************
* Function: keypad_init
* Inputs: None
* Outputs: None.
* Description: This function initializes the pins connected to the
* keypad and sets them to high.
************************************************************************/
void keypad_init(void){
    P4->DIR&=~0xFF;//sets all pins connected to input
    P4->REN|=0x0F;//enables pull up resistor
    P4->OUT|=0x0F;//sets the rows to high
}

/************************************************************************
* Function: keypad_getkey
* Inputs: None
* Outputs: None.
* Description: This function was sourced from the lecture slides. The
* purpose of this function is to detect if a button on the keypad is
* pressed and send the information regarding the button press back
* to main.
************************************************************************/
int keypad_getkey(void){
    int row,col;
for(col=0;col<3;col++){//for loop used to detect if any buttons have been pressed
    P4->DIR=0x00;//sets all columns to inputs
    P4->DIR|=BIT(4+col);//set current column to output
    P4->OUT&=~BIT(4+col);//set current column to low
    delay_milli(10);
    row=P4->IN & 0x0F;//reads the value of all rows
    while(!(P4IN & BIT0)|!(P4IN & BIT1)|!(P4IN & BIT2)|!(P4IN & BIT3));//iterates while a button is pressed
    if(row!= 0x0F){break;}
}
P4->DIR = 0x00;//sets columns to input
if(col == 3) return 0;//theses next few lines return the button that has been pressed back to main
if(row == 0x0E) return col+1;
if(row == 0x0D) return col+4;
if(row == 0x0B) return col+7;
if(row == 0x07) return col+10;
}
