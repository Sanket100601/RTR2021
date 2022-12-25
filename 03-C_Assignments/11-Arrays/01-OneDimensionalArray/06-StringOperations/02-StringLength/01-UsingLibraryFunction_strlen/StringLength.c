#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //variable declarations 
    char chArray_SSP[MAX_STRING_LENGTH]; // A Character Array Is A String
    int iStringLength_SSP = 0;

    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(chArray_SSP, MAX_STRING_LENGTH,stdin);
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", chArray_SSP);
    
    // *** STRING LENGTH ***
    printf("\n\n");
    iStringLength_SSP = strlen(chArray_SSP);
    printf("Length Of String Is = %d Characters !!!\n\n", iStringLength_SSP);
    return(0);
}
/*
Enter A String : 

SANKET


String Entered By You Is :

SANKET



Length Of String Is = 7 Characters !!!
*/