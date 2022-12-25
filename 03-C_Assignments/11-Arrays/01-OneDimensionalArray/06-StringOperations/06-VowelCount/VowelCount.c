#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype
    int MyStrlen(char[]);
    
    //variable declarations 
    char chArray_SSP[MAX_STRING_LENGTH]; // A Character Array Is A String
    int iStringLength_SSP;
    int count_A_SSP = 0, count_E_SSP = 0, count_I_SSP = 0, count_O_SSP = 0, count_U_SSP = 0; // Initial Counts = 0
    int i_SSP;

    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(chArray_SSP, MAX_STRING_LENGTH,stdin);
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", chArray_SSP);
    iStringLength_SSP = MyStrlen(chArray_SSP);
    for (i_SSP = 0; i_SSP < iStringLength_SSP; i_SSP++)
    {
        switch (chArray_SSP[i_SSP])
        {
            case 'A':
            case 'a':
            count_A_SSP++;
            break;

            case 'E':
            case 'e':   
            count_E_SSP++;
            break;

            case 'I':
            case 'i':
            count_I_SSP++;
            break;

            case 'O':
            case 'o':   
            count_O_SSP++;
            break;

            case 'U':
            case 'u':

            count_U_SSP++;
            break;
        
            default:
            break;
        }
    }
    printf("\n\n");
    printf("In The String Entered By You, The Vowels And The Number Of Their Occurences Are Are Follows : \n\n");
    printf("'A' Has Occured = %d Times !!!\n\n", count_A_SSP);
    printf("'E' Has Occured = %d Times !!!\n\n", count_E_SSP);
    printf("'I' Has Occured = %d Times !!!\n\n", count_I_SSP);
    printf("'O' Has Occured = %d Times !!!\n\n", count_O_SSP);
    printf("'U' Has Occured = %d Times !!!\n\n", count_U_SSP);
    
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



In The String Entered By You, The Vowels And The Number Of Their Occurences Are Are Follows :

'A' Has Occured = 1 Times !!!

'E' Has Occured = 1 Times !!!

'I' Has Occured = 0 Times !!!

'O' Has Occured = 0 Times !!!

'U' Has Occured = 0 Times !!!
*/