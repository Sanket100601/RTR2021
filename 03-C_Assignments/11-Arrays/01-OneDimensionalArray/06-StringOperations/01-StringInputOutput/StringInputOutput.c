#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //variable declarations 
    char chArray_SSP[MAX_STRING_LENGTH]; // A Character Array Is A String
    
    //code      
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(chArray_SSP, MAX_STRING_LENGTH,stdin); 

    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", chArray_SSP);
    return(0);
}
/*
Enter A String : 

SANKET


String Entered By You Is :

SANKET
*/