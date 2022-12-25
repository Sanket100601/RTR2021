#include <stdio.h>
#include <string.h> // for strrev()
#define MAX_STRING_LENGTH 512
int main(void)
{
    //variable declarations 
    char chArray_Original_SSP[MAX_STRING_LENGTH]; // A Character Array Is A String

    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(chArray_Original_SSP, MAX_STRING_LENGTH,stdin);
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
    printf("%s\n", chArray_Original_SSP);
    printf("\n\n");
    printf("The Reversed String (i.e : 'chArray_Reverse[]') Is : \n\n");
    printf("%s\n", strrev(chArray_Original_SSP));
    return(0);
}