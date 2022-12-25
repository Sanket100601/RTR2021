#include <stdio.h>
#include<string.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //variable declarations 
    char chArray_One[MAX_STRING_LENGTH], chArray_Two[MAX_STRING_LENGTH]; // A Character Array Is A String
    
    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter First String : \n\n");
    fgets(chArray_One, MAX_STRING_LENGTH,stdin);
    
    printf("\n\n");
    printf("Enter Second String : \n\n");
    fgets(chArray_Two, MAX_STRING_LENGTH,stdin);
    
    // *** STRING CONCAT ***
    printf("\n\n");
    printf("****** BEFORE CONCATENATION ******");
    printf("\n\n");
    printf("The Original First String Entered By You (i.e : 'chArray_One[]') Is : \n\n");
    printf("%s\n", chArray_One);    
    
    printf("\n\n");
    printf("The Original Second String Entered By You (i.e : 'chArray_Two[]') Is : \n\n");
    printf("%s\n", chArray_Two);
    strcat(chArray_One, chArray_Two);
    
    printf("\n\n");
    printf("****** AFTER CONCATENATION ******");
    
    printf("\n\n");
    printf("'chArray_One[]' Is : \n\n");
    printf("%s\n", chArray_One);
    
    printf("\n\n");
    printf("'chArray_Two[]' Is : \n\n");
    printf("%s\n", chArray_Two);
    return(0);
}
/*
Enter First String : 

SANKET


Enter Second String :

PAWAR


****** BEFORE CONCATENATION ******

The Original First String Entered By You (i.e : 'chArray_One[]') Is :

SANKET



The Original Second String Entered By You (i.e : 'chArray_Two[]') Is :

PAWAR



****** AFTER CONCATENATION ******

'chArray_One[]' Is :

SANKET
PAWAR



'chArray_Two[]' Is :

PAWAR
*/