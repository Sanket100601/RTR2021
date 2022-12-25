#include <stdio.h>
#define MAX_STRING_LENGTH 512
#define SPACE ' '
#define FULLSTOP '.'
#define COMMA ','
#define EXCLAMATION '!'
#define QUESTION_MARK '?'
int main(void)
{
    //function prototype
    int MyStrlen(char[]);
    char MyToUpper(char);

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
        if (i_SSP == 0) //First Letter Of Any Sentence Must Be A CAPITAL LETTER
            chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP] = MyToUpper(chArray_SSP[i_SSP]);
        else if (chArray_SSP[i_SSP] == SPACE) //First Letter Of Every Word In The Sentence Must Be A CAPITAL LETTER. Words Are Separated By Spaces.
        {
            chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP] = chArray_SSP[i_SSP];
            chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP + 1] = MyToUpper(chArray_SSP[i_SSP + 1]);
            //SINCE, ALREADY TWO CHARACTERS (AT INDICES 'i' AND i + 1 HAVE BEEN CONSIDERED IN THIS else-if BLOCK...WE ARE EXTRA-INCREMENTING 'i' AND 'j' BY 1
            j_SSP++;
            i_SSP++;
        }
        else if ((chArray_SSP[i_SSP] == FULLSTOP || chArray_SSP[i_SSP] == COMMA || chArray_SSP[i_SSP] == EXCLAMATION || chArray_SSP[i_SSP] == QUESTION_MARK) && (chArray_SSP[i_SSP] != SPACE)) //First Letter Of Every Word After Punctuation Mark, In The Sentence Must Be A CAPITAL LETTER. Words Are Separated By Punctuations.
        {
            chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP] = chArray_SSP[i_SSP];
            chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP + 1] = SPACE;
            chArray_CapitalizedFirstLetterOfEveryWord_SSP[j_SSP + 2] = MyToUpper(chArray_SSP[i_SSP + 1]);
            // SINCE, ALREADY TWO CHARACTERS (AT INDICES 'i' AND i + 1 HAVE BEEN CONSIDERED IN THIS else-if BLOCK...WE ARE EXTRA-INCREMENTING 'i' BY 1
            // SINCE, ALREADY THREE CHARACTERS (AT INDICES 'j' AND (j + 1) AND (j+ 2) HAVE BEEN CONSIDERED IN THIS else-if BLOCK...WE ARE EXTRAINCREMENTING 'j' BY 2
            j_SSP = j_SSP + 2;
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

char MyToUpper(char ch)
{
    //variable declaration
    int num_SSP;
    int c_SSP;
    //code
    // ASCII VALUE OF 'a' (97) - ASCII VALUE OF 'A' (65) = 32
    // THIS SUBTRACTION WILL GIVE THE EXACT DIFFERENCE BETWEEN THE UPPER AND LOWER CASE COUNTERPARTS OF EACH LETTER OF THE ALPHABET
    // IF THIS DIFFERENCE IS SUBTRACTED FROM THE ASCII VALUE OF A LOWER CASE LETTER, THE RESULTANT ASCII VALUE WILL BE THAT OF ITS UPPER CASE COUNTERPART, HENCE, HELPING US TO FIND ITS UPPER CASE LETTER !!!
    // ASCII VALUES OF 'a' to 'z' => 97 TO 122
    // ASCII VALUES OF 'A' to 'Z' => 65 TO 90
    num_SSP = 'a' - 'A';
    if ((int)ch >= 97 && (int)ch <= 122)
    {
        c_SSP = (int)ch - num_SSP;
        return((char)c_SSP);
    }
    else
    return(ch);
}
/*
Enter A String : 

Sanket


String Entered By You Is :

Sanket



String After Capitalizing First Letter Of Every Word :

Sanket
*/
