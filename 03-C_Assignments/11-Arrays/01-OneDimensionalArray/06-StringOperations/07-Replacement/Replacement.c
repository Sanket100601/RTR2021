// *** THSI PROGRAM REPLACES ALL VOWELS IN THE INPUT STRING WITH THE * (asterisk) SYMBOL ***
// *** FOR EXAMPLE, ORIGINAL STRING 'Dr. Vijay Dattatray Gokhale ASTROMEDICOMP' WILL BECOME 'Dr. V*j*y D*tt*tr*y* G*kh*le *STR*M*D*C*MP' 
#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype
    int MyStrlen(char[]);
    void MyStrcpy(char[], char[]);
    
    //variable declarations 
    char chArray_Original_SSP[MAX_STRING_LENGTH], chArray_VowelsReplaced_SSP[MAX_STRING_LENGTH]; // A Character Array Is A String
    int iStringLength_SSP;
    int i_SSP;
    
    //code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(chArray_Original_SSP, MAX_STRING_LENGTH,stdin);
    
    // *** STRING OUTPUT ***
    MyStrcpy(chArray_VowelsReplaced_SSP, chArray_Original_SSP);
    iStringLength_SSP = MyStrlen(chArray_VowelsReplaced_SSP);
    for (i_SSP = 0; i_SSP < iStringLength_SSP; i_SSP++)
    {
        switch (chArray_VowelsReplaced_SSP[i_SSP])
        {
            case 'A':
            case 'a':
            case 'E':
            case 'e':
            case 'I':
            case 'i':
            case 'O':
            case 'o':
            case 'U':
            case 'u':
            chArray_VowelsReplaced_SSP[i_SSP] = '*';
            break;
            default:
            break;
        }
    }
    
    // *** STRING OUTPUT ***
    printf("\n\n"); 
    printf("String Entered By You Is : \n\n");
    printf("%s\n", chArray_Original_SSP);   
    printf("\n\n");
    printf("String After Replacement Of Vowels By * Is : \n\n");
    printf("%s\n", chArray_VowelsReplaced_SSP);
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
void MyStrcpy(char str_destination[], char str_source[])
{
    //function prototype
    int MyStrlen(char[]);
    
    //variable declarations 
    int iStringLength_SSP = 0;
    int j_SSP;
    
    //code
    iStringLength_SSP = MyStrlen(str_source);
    for (j_SSP = 0; j_SSP < iStringLength_SSP; j_SSP++)
    str_destination[j_SSP] = str_source[j_SSP];
    str_destination[j_SSP] = '\0';
}
/*
Enter A String : 

SANKET


String Entered By You Is :

SANKET



String After Replacement Of Vowels By * Is :

S*NK*T
*/
