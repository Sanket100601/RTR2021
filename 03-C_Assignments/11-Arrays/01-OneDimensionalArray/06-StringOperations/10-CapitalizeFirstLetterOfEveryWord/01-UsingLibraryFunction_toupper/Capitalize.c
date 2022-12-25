#include <stdio.h>
#include <ctype.h> //for toupper()
#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype
    int MyStrlen(char[]);

    //variable declarations 
    char chArray_SSP[MAX_STRING_LENGTH], chArray_CapitalizedFirstLetterOfEveryWord_SSP[MAX_STRING_LENGTH]; // A Character Array Is A String
    int iStringLength_SSP;
    int i_SSP, j_SSP;

    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(chArray_SSP, MAX_STRING_LENGTH,stdin);
    iStringLength_SSP = MyStrlen(chArray_SSP);
    j_SSP = 0;
    for (i_SSP = 0; i_SSP < iStringLength_SSP; i_SSP++)
    {
        if (i_SSP == 0)
        chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP] = toupper(chArray_SSP[i_SSP]);
    else if (chArray_SSP[i_SSP] == ' ')
    {
        chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP] = chArray_SSP[i_SSP];
        chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP + 1] = toupper(chArray_SSP[i_SSP + 1]);
        //SINCE, ALREADY TWO CHARACTERS (AT INDICES 'i' AND i + 1 HAVE BEEN CONSIDERED IN THIS else-if BLOCK...WE ARE EXTRA-INCREMENTING 'i' AND 'j' BY 1
        j_SSP++;
        i_SSP++;
    }
    else
        chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP] = chArray_SSP[i_SSP];
        j_SSP++;
    }
    chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP] = '\0';
    // *** STRING OUTPUT ***    
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", chArray_SSP);
    printf("\n\n");
    printf("String After Capitalizing First Letter Of Every Word : \n\n");
    printf("%s\n", chArray_CapitalizedFirstLetterOfEveryWord_SSP);

    return(0);
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


String Entered By You Is :

SANKET



String After Capitalizing First Letter Of Every Word :

SANKET

*/