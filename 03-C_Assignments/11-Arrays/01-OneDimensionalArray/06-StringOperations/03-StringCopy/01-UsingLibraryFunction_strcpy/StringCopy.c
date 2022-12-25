#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //variable declarations 
    char chArray_Original[MAX_STRING_LENGTH], chArray_Copy[MAX_STRING_LENGTH]; // A Character Array Is A String
    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(chArray_Original, MAX_STRING_LENGTH,stdin);
    
    // *** STRING COPY ***
    strcpy(chArray_Copy, chArray_Original);
   
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
    printf("%s\n", chArray_Original);
    printf("\n\n");
    printf("The Copied String (i.e : 'chArray_Copy[]') Is : \n\n");
    printf("%s\n", chArray_Copy);
    return(0);
}
/*
Enter A String : 

SANKET


The Original String Entered By You (i.e : 'chArray_Original[]') Is :

SANKET



The Copied String (i.e : 'chArray_Copy[]') Is :

SANKET
*/