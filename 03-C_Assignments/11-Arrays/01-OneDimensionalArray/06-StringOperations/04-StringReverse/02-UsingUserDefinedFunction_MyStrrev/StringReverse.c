#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype    
    void MyStrrev(char[], char[]);
    //variable declarations 
    char chArray_Original_SSP[MAX_STRING_LENGTH], chArray_Reversed_SSP[MAX_STRING_LENGTH]; // A Character Array Is A String
   
    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(chArray_Original_SSP, MAX_STRING_LENGTH,stdin);
    
    // *** STRING REVERSE ***
    MyStrrev(chArray_Reversed_SSP, chArray_Original_SSP);
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
    printf("%s\n", chArray_Original_SSP);
    printf("\n\n");
    printf("The Reversed String (i.e : 'chArray_Reversed[]') Is : \n\n");
    printf("%s\n", chArray_Reversed_SSP);
    
    return(0);
}

void MyStrrev(char str_destination[], char str_source[])
{
    //function prototype
    int MyStrlen(char[]);

    //variable declarations 
    int iStringLength_SSP = 0;
    int i_SSP, j_SSP, len_SSP;
   
    //code
    iStringLength_SSP = MyStrlen(str_source);
   
    // ARRAY INDICES BEGIN FROM 0, HENCE, LAST INDEX WILL ALWAYS BE (LENGTH - 1)
    len_SSP = iStringLength_SSP - 1;
    // WE NEED TO PUT THE CHARACTER WHICH IS AT LAST INDEX OF 'str_source' TO THE FIRST INDEX OF 'str_destination'
    // AND SECOND-LAST CHARACTER OF 'str_source' TO THE SECOND CHARACTER OF 'str_destination' and so on...
    for (i_SSP = 0, j_SSP = len_SSP; i_SSP < iStringLength_SSP, j_SSP >= 0; i_SSP++, j_SSP--)
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
