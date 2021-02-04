/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 01/29/21
* Project: Lab 2
* File: PartOne.c
* Description: This is a program that is designed to quickly and easily identify the proper
* resistor color code based on the input of the resistance of said resistor. this program will
* accept a number between 0 and 99 million and output the corresponding color code that would
* match resistance of the resistor so as to allow the user to identify the correct resistor.
*
**************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

void prompt(void);//functions prototypes required
void calcResistorColors(int);
void color(int);//custom function to print the colors of the bands of the resistor
void getColorBands(char*,char*,char*,char*);
void calcResistance(char,char,char,char);

int main()
{
    int stat=0;
    int resistance;
    int desc;
    do{
    prompt();//function call to print the resistor values table
    printf("Would you like to run the program again(0=no,1=yes)");
    scanf("%d",&desc);
    }while(desc!=0);
}
/************************************************************************
* Function: prompt
* Inputs: None
* Outputs: Prints resistor values table and prompts for user input
* Description: the prompt function serves two purposes. the first being
* to print out the resistor values table and the second being to prompt
* the user to decide which version of the program to run and accept
* input for each portion of the program.
************************************************************************/
void prompt(void){//function that prints the resistors values table
    int dec,s;
    int stat=0;
    int resistance;
    int desc;
    char B1,B2,B3,B4,B5,B6,B7,B8;
 printf("--------------------Resistor Codes-----------------------\n");
 printf("|Character| Color | Band 1 & 2 |  Band 3   |  Band 4    |\n");
 printf("|    K    | Black |      0     |*1         |  +/-1%%     |\n");
 printf("|    N    | Brown |      1     |*10        |  +/-2%%     |\n");
 printf("|    R    | Red   |      2     |*100       |            |\n");
 printf("|    O    | Orange|      3     |*1,000     |            |\n");
 printf("|    Y    | Yellow|      4     |*10,000    |            |\n");
 printf("|    G    | Green |      5     |*100,000   |  +/-0.5%%   |\n");
 printf("|    B    | Blue  |      6     |*1,000,000 |  +/-0.25%%  |\n");
 printf("|    V    | Violet|      7     |*10,000,000|  +/-0.1%%   |\n");
 printf("|    E    | Grey  |      8     |           |  +/-0.05%%  |\n");
 printf("|    W    | White |      9     |           |            |\n");
 printf("|    D    | Gold  |            |           |  +/-5%%     |\n");
 printf("|    S    | Silver|            |           |  +/-10%%    |\n");
 printf("-------------------------------------------------------\n\n");
do{//do while loop to run the program continually allowing the user to choose the method of evaluation
    printf("Choose which resistor function you would like to preform:\n");
    printf("\t(1)Determine color code for given resistance value.\n");
    printf("\t(2)Determine resistance value for given color code.\n");
        s=scanf("%d",&dec);
        if(s==0||dec>2){//ensures the input is within the specified range
        printf("Error: incompatable input\n");
        }
        fflush(stdin);
        }while(s==0||dec>2);

if(dec==1){
        printf("Please enter a resistance value between 0-99000000:\n");
        do{//error checking loop
        stat=scanf("%d",&resistance);
        if(stat==0||resistance>99000000){//ensures the input is within the specified range
        printf("Error: incompatable input\n");
        printf("Please enter a resistance value between 0-99000000:\n");
        }
        fflush(stdin);
        }while(stat==0||resistance>99000000);
calcResistorColors(resistance);//function call to separate the input number into usable single digit numbers
}

if(dec==2){//conditional that will preform part one of lab 2
getColorBands(&B1,&B2,&B3,&B4);
calcResistance(B1,B2,B3,B4);
}
}
/************************************************************************
* Function: calcResistorColors
* Inputs: resistance value
* Outputs: none
* Description: this functions job is to break down a resistance value to
* usable single digit numbers which can be directly translated into
* band colors in another function.
************************************************************************/
void calcResistorColors(int resistance){
int val[9];
int i=resistance;
int j;//multiplier
int k;//first band
int r;//second band
int mult;
for(j=0;i>=100;j++){//counts the zeros to determain multiplier
    mult=i/10;
    i=mult;
}
k=i/10;//math to solve for first band
r=i-(k*10);//math to solve for second band
color(k);//function calls to change the number to a color
printf("-");
color(r);
printf("-");
color(j);
printf("\n");
}
/************************************************************************
* Function: color
* Inputs: band number/color
* Outputs: Prints band colors to the screen
* Description: this function takes the broken down resistor numbers
* from calcResistorColors and prints the corresponding color to the number
************************************************************************/
void color(int band){//converts numbers to colors

if(band==0){printf("Black");}
if(band==1){printf("Brown");}
if(band==2){printf("Red");}
if(band==3){printf("Orange");}
if(band==4){printf("Yellow");}
if(band==5){printf("Green");}
if(band==6){printf("Blue");}
if(band==7){printf("Violet");}
if(band==8){printf("Grey");}
if(band==9){printf("White");}
}
/************************************************************************
* Function: getColorBands
* Inputs: pointers to band colors
* Outputs: returns the band color characters
* Description: this function allows the user to input a character that represents
* a band color and then stores the value of that character in a pointer
* allowing the function to return all four band colors at once.
************************************************************************/
void getColorBands(char* B1,char* B2,char* B3,char* B4){//aquires user input
char one,two,three,four;
int s;

        do{  printf("Please enter the color code character for Band 1:\n");
        scanf("%c",&one);
        if(one!=75&&one!=78&&one!=82&&one!=79&&one!=89&&one!=71&&one!=66&&one!=86&&one!=69&&one!=87){//ensures the input is within the specified range
        printf("Error: incompatable input\n");
        }
        fflush(stdin);
        }while(one!=75&&one!=78&&one!=82&&one!=79&&one!=89&&one!=71&&one!=66&&one!=86&&one!=69&&one!=87);

         do{  printf("Please enter the color code character for Band 2:\n");
        scanf("%c",&two);
        if(two!=75&&two!=78&&two!=82&&two!=79&&two!=89&&two!=71&&two!=66&&two!=86&&two!=69&&two!=87){//ensures the input is within the specified range
        printf("Error: incompatable input\n");
        }
        fflush(stdin);
        }while(two!=75&&two!=78&&two!=82&&two!=79&&two!=89&&two!=71&&two!=66&&two!=86&&two!=69&&two!=87);

         do{  printf("Please enter the color code character for Band 3:\n");
        scanf("%c",&three);
        if(three!=75&&three!=78&&three!=82&&three!=79&&three!=89&&three!=71&&three!=66&&three!=86){//ensures the input is within the specified range
        printf("Error: incompatable input\n");
        }
        fflush(stdin);
        }while(three!=75&&three!=78&&three!=82&&three!=79&&three!=89&&three!=71&&three!=66&&three!=86);

         do{  printf("Please enter the color code character for Band 4:\n");
        scanf("%c",&four);
        if(four!=75&&four!=78&&four!=71&&four!=66&&four!=86&&four!=69&&four!=68&&four!=83){//ensures the input is within the specified range
        printf("Error: incompatable input\n");
        }
        fflush(stdin);
        }while(four!=75&&four!=78&&four!=71&&four!=66&&four!=86&&four!=69&&four!=68&&four!=83);
*B1=one;//returns the entered values using pointers
*B2=two;
*B3=three;
*B4=four;
}
/************************************************************************
* Function: calcResistance
* Inputs: band color characters
* Outputs: Prints the resistance to the screen
* Description: this function takes the band color characters and prints
* the corresponding resistance value of the resistor.
************************************************************************/
void calcResistance(char B1,char B2,char B3,char B4){//function that accepts the characters corresponding to band colors and converts them to a resistance value
if(B1==75){printf("0");}
if(B1==78){printf("1");}
if(B1==82){printf("2");}
if(B1==79){printf("3");}
if(B1==89){printf("4");}
if(B1==71){printf("5");}
if(B1==66){printf("6");}
if(B1==86){printf("7");}
if(B1==69){printf("8");}
if(B1==87){printf("9");}

if(B2==75){printf("0");}
if(B2==78){printf("1");}
if(B2==82){printf("2");}
if(B2==79){printf("3");}
if(B2==89){printf("4");}
if(B2==71){printf("5");}
if(B2==66){printf("6");}
if(B2==86){printf("7");}
if(B2==69){printf("8");}
if(B2==87){printf("9");}


if(B3==78){printf("0");}
if(B3==82){printf("00");}
if(B3==79){printf("000");}
if(B3==89){printf("0000");}
if(B3==71){printf("00000");}
if(B3==66){printf("000000");}
if(B3==86){printf("0000000");}

if(B4==75){printf("+/- 1%%\n");}
if(B4==78){printf("+/- 2%%\n");}
if(B4==71){printf("+/- 0.5%%\n");}
if(B4==66){printf("+/- 0.25%%\n");}
if(B4==86){printf("+/- 0.1%%\n");}
if(B4==69){printf("+/- 0.05%%\n");}
if(B4==68){printf("+/- 5%%\n");}
if(B4==83){printf("+/- 10%%\n");}
}
