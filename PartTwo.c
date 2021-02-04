/**************************************************************************************
* Author: Seth Nida
* Course: EGR 226 - 905
* Date: 01/29/21
* Project: Lab 2
* File: PartTwo.c
* Description: This program is a search engine designed to accept a csv file containing
* a list of books and some details regarding those books. then the user can enter either
* the title of the book the authors name of the ISBN of the book in order to find any and
* all books containing any of the key words or numbers entered by the user.
*
**************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{//structured variable of which stores all information on the books in the list
char title[255];
char author_name[50];
char ISBN[10];
int pages;
int year_published;
}book;


int parse_file(char filename[], book book_array[]);//function prototypes used to allow for the proper operation of the search engine
int print_book(book book_array[],int i);
int search_title(book book_array[],int num,char title[]);
int search_author(book book_array[],int num,char author[]);
int search_ISBN(book book_array[],int num,char spinenum[]);

int main()
{book book_array[360];//sets the structured variable to an array of 360 elements, ie the program can sore up to 360 books
int num=360;
char title[90];
char author[90];
char spinenum[20];
int desc;
char stat;
char filename[30]="Booklist.csv";

parse_file(filename,book_array);//function call to parse the file and sort all information from the csv file into an array of a structured variable

do{//loop to have the program run until the user terminates it
printf("What would you like to search by? ( 0=Title, 1=Author, 2=ISBN, 3=Terminate program )\n");
        do{//error checking loop
        stat=scanf("%d",&desc);
        if(desc<0||desc>3){//ensures the input is within the specified range
        printf("Error: incompatable input\n");
        printf("What would you like to search by? ( 0=Title, 1=Author, 2=ISBN, 3=Terminate program )\n");
        }
        fflush(stdin);
        }while(desc<0||desc>3);


if(desc==0){//evaluates to true if the user decides to search by title
    printf("Please enter the book title\n");
    scanf("%s",&title);//accepts the title name entered by the user regardless of what it may be
    search_title(book_array,num,title);//passes the name of the title of the book to a search function
}

if(desc==1){//evaluates to true if the user decides to search by author
    printf("Please enter the book author\n");
    scanf("%s",&author);//accepts the author name entered by the user regardless of what it may be
    search_author(book_array,num,author);//passes the name of the author of the book to a search function
}

if(desc==2){//evaluates to true if the user decides to search by ISBN
    printf("Please enter the book ISBN\n");
    scanf("%s",&spinenum);//accepts the ISBN number regardless of what it may be
    search_ISBN(book_array,num,spinenum);//passes the ISBN number o a search function
}
if(desc==3){printf("\nGoodbye\n");}//terminates the program if the user enters 3
}while(desc!=3);

}

/************************************************************************
* Function: parse file
* Inputs: filename and structured variable
* Outputs: populates the structured variable
* Description: this function reads all values form the csv file and
* stores the values contained there into the structured variable
************************************************************************/
int parse_file(char filename[], book book_array[]) {//function to input information from a csv file into the structured variable

 FILE* infile = fopen(filename, "r"); // Attempt to open file
 if (infile == NULL) // Return 0 (failure) if file could not open
 return 0;

 char buffer[512]; // Create temporary string buffer variable
 int i = 0;
 int j; // Indexer for book array

 while (fgets(buffer, 512, infile)) { // Loop collecting each line from the file
 char * ptr = strtok(buffer,",");// Parse string by commas and newline
 if(strcmp(ptr,"N/A")) // Validate string
 strcpy(book_array[i].title,ptr);// First parse is title

 ptr = strtok(NULL,",\n");
 if(strcmp(ptr,"N/A")) // Validate string
 strcpy(book_array[i].author_name,ptr);//second parse is the authors name

 ptr = strtok(NULL,",\n");
 strcpy(book_array[i].ISBN,ptr);//third parse is the books ISBN

 ptr = strtok(NULL,",\n");
 j=strtol(ptr,NULL,10);//stores the number of pages into j
 book_array[i].pages=j;//stores the page number into the structured variable

 ptr = strtok(NULL,",\n");
 j=strtol(ptr,NULL,10);//stores the year published in j
 book_array[i].year_published=j;//stores the year published into the structured variable

 i++;
 }
 return 1;
}
/************************************************************************
* Function: print book
* Inputs: structured variable and location in the array
* Outputs: prints the information regarding a particular book to the screen
* Description: this function prints all stored information on a book to the
* screen.
************************************************************************/
int print_book(book book_array[],int i){//function that prints all information related to a book when called
printf("Title:");
printf("%s\n",book_array[i].title);//prints the value stored in the structured variable
printf("Author:");
printf("%s\n",book_array[i].author_name);//prints the value stored in the structured variable
printf("ISBN:");
printf("%s\n",book_array[i].ISBN);//prints the value stored in the structured variable
if(book_array[i].pages!=0){
printf("Pages:");
printf("%d\n",book_array[i].pages);}
if(book_array[i].pages==0){printf("Pages: N/A\n");}//deals with null values and corrects the improper stored value
if(book_array[i].year_published!=0){
printf("Year published:");
printf("%d\n\n",book_array[i].year_published);}
if(book_array[i].year_published==0){printf("Year published: N/A\n");}//deals with null value and corrects for improperly stored value
}
/************************************************************************
* Function: search title
* Inputs: structured variable, list length, and user input
* Outputs: none
* Description: this function loops through all books contained in the list
* searching for similar key words and if any are found they information
* on that book is passed to print book.
************************************************************************/
int search_title(book book_array[],int num,char title[]){//function that searches for any key words in all book titles that match user input
int j,i;
for(i=0;i<num;i++){//loops through all titles
if(strstr(book_array[i].title,title)){//checks for matching words
    print_book(book_array,i);//if a match is found the print function is called and the book information will be printed to the screen
}
}
}
/************************************************************************
* Function: search title
* Inputs: structured variable, list length, and user input
* Outputs: none
* Description: this function loops through all books contained in the list
* searching for similar key words and if any are found they information
* on that book is passed to print book.
************************************************************************/
int search_author(book book_array[],int num,char author[]){//function that searches for any key words in all author names that match user input
int j,i;
for(i=0;i<num;i++){//loops through all author names
if(strstr(book_array[i].author_name,author)){//checks for matching words
    print_book(book_array,i);//if a match is found the print function is called and the book information will be printed to the screen
}
}
}
/************************************************************************
* Function: search title
* Inputs: structured variable, list length, and user input
* Outputs: none
* Description: this function loops through all books contained in the list
* searching for similar key words and if any are found they information
* on that book is passed to print book.
************************************************************************/
int search_ISBN(book book_array[],int num,char spinenum[]){//function that searches for any key numbers in all ISBN's that match user input
int j,i;
for(i=0;i<num;i++){//loops through all ISBN's
if(strstr(book_array[i].ISBN,spinenum)){//checks for matching numbers
    print_book(book_array,i);//if a match is found the print function is called and the book information will be printed to the screen
}
}
}
