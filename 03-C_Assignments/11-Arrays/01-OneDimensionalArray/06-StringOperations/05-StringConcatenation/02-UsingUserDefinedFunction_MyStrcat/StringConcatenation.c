#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype
    void MyStrcat(char[], char[]);
    
    //variable declarations 
    char chArray_One_SSP[MAX_STRING_LENGTH], chArray_Two_SSP[MAX_STRING_LENGTH]; // A Character Array Is A String
    
    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter First String : \n\n");
    fgets(chArray_One_SSP, MAX_STRING_LENGTH,stdin);
    
    printf("\n\n");
    printf("Enter Second String : \n\n");
    fgets(chArray_Two_SSP, MAX_STRING_LENGTH,stdin);
    
    // *** STRING CONCAT ***
    printf("\n\n");
    printf("****** BEFORE CONCATENATION ******");
    
    printf("\n\n");
    printf("The Original First String Entered By You (i.e : 'chArray_One[]') Is : \n\n");
    printf("%s\n", chArray_One_SSP);
    
    printf("\n\n");
    printf("The Original Second String Entered By You (i.e : 'chArray_Two[]') Is :\n\n");
    printf("%s\n", chArray_Two_SSP);
    MyStrcat(chArray_One_SSP, chArray_Two_SSP);
    
    printf("\n\n"); 
    printf("****** AFTER CONCATENATION ******");
    
    printf("\n\n");
    printf("'chArray_One[]' Is : \n\n");
    printf("%s\n", chArray_One_SSP);

    printf("\n\n");
    printf("'chArray_Two[]' Is : \n\n");
    printf("%s\n", chArray_Two_SSP);
    return(0);
}
void MyStrcat(char str_destination[], char str_source[])
{
    //function prototype
    int MyStrlen(char[]);
    
    //variable declarations 
    int iStringLength_Source_SSP = 0, iStringLength_Destination_SSP = 0;
    int i_SSP, j_SSP;
    
    //code
    iStringLength_Source_SSP = MyStrlen(str_source);
    iStringLength_Destination_SSP = MyStrlen(str_destination);
    // ARRAY INDICES BEGIN FROM 0, HENCE, LAST VALID INDEX OF ARRAY WILL ALWAYS BE (LENGTH - 1)
    // SO, CONCATENATION MUST BEGIN FROM INDEX NUMBER EQUAL TO LENGTH OF THE ARRAY 'str_destination'
    // WE NEED TO PUT THE CHARACTER WHICH IS AT FIRST INDEX OF 'str_source' TO THE (LAST INDEX + 1) OF 'str_destination'
    for (i_SSP = iStringLength_Destination_SSP, j_SSP = 0; j_SSP < iStringLength_Source_SSP; i_SSP++, j_SSP++)
    {
    str_destination[i_SSP] = str_source[j_SSP]; 
    } 
    str_destination[i_SSP] = '\0';
}
int MyStrlen(char str[])
{
    //variable declarations
    int j_SSP;
    int string_length_SSP = 0;
    //code
    // *** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER ( \0 ) ***
    for (j_SSP = 0; j_SSP < MAX_STRING_LENGTH; j_SSP++) 
    {
    if (str[j_SSP] == '\0')
    break;
    else
    string_length_SSP++;
    }
    return(string_length_SSP);
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