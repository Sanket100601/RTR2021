#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype
    int MyStrlen(char[]);
    void MyStrcpy(char[], char[]);
   
    //variable declarations 
    char chArray_SSP[MAX_STRING_LENGTH]; // A Character Array Is A String   
    int iStringLength_SSP;
    int i_SSP;
    int word_count_SSP = 0, space_count_SSP = 0;

    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(chArray_SSP, MAX_STRING_LENGTH,stdin);
    iStringLength_SSP = MyStrlen(chArray_SSP);
    for (i_SSP = 0; i_SSP < iStringLength_SSP; i_SSP++)
    {
        switch (chArray_SSP[i_SSP])
        {
            case 32: //32 IS THE ASCII VALUE FOR SPACE (' ') CHARACTER 
            space_count_SSP++;
            break;
            default:
            break;
        }
    }
    word_count_SSP = space_count_SSP + 1;
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", chArray_SSP);
    printf("\n\n");
    printf("Number Of Spaces In The Input String = %d\n\n", space_count_SSP);
    printf("Number Of Words In The Input String = %d\n\n", word_count_SSP);
    
    return(0);
}

int MyStrlen(char str[])
{
    //variable declarations
    int j;
    int string_length = 0;

    //code
    // *** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER ( \0 ) ***
    for (j = 0; j < MAX_STRING_LENGTH; j++) 
    {
        if (str[j] == '\0')
        break;
        else
        string_length++;
    }
    return(string_length);
}
/*
Enter A String : 

SANKET


String Entered By You Is :

SANKET



Number Of Spaces In The Input String = 0

Number Of Words In The Input String = 1
*/
