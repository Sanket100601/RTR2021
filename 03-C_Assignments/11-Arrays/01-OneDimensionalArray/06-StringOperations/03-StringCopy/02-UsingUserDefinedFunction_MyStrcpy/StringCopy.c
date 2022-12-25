#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //variable declarations 
    char chArray_Original_SSP[MAX_STRING_LENGTH], chArray_Copy_SSP[MAX_STRING_LENGTH]; // A Character Array Is A String
    
    //function prototype
    void MyStrcpy(char[], char[]);

    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(chArray_Original_SSP, MAX_STRING_LENGTH,stdin);

    // *** STRING COPY ***
    MyStrcpy(chArray_Copy_SSP, chArray_Original_SSP);
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
    printf("%s\n", chArray_Original_SSP);
    printf("\n\n");
    printf("The Copied String (i.e : 'chArray_Copy[]') Is : \n\n");
    printf("%s\n", chArray_Copy_SSP);
    
    return(0);
}
void MyStrcpy(char str_destination[], char str_source[])
{
    //function prototype
    int MyStrlen_SSP(char[]);
    
    //variable declarations     
    int iStringLength_SSP = 0;
    int j_SSP;
    
    //code
    iStringLength_SSP = MyStrlen(str_source);
    for (j_SSP = 0; j_SSP < iStringLength_SSP; j_SSP++)
    str_destination[j_SSP] = str_source[j_SSP];
    str_destination[j_SSP] = '\0';
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
Enter A String : 

SANKET


The Original String Entered By You (i.e : 'chArray_Original[]') Is :

SANKET



The Copied String (i.e : 'chArray_Copy[]') Is :

SANKET
*/