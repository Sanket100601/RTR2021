#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype
    int MyStrlen(char *);

    //variable declarations 
    char *chArray_SSP = NULL; //Character Array Can Be Represented By A char pointer to Mark The Base Address (char *)
    int iStringLength_SSP = 0;

    //code
    printf("\n\n");
    chArray_SSP = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray_SSP == NULL)
    {   
        printf("MEMORY ALOCATION TO CHARACTER ARRAY FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }

    // *** STRING INPUT ***
    printf("Enter A String : \n\n");
    fgets(chArray_SSP, MAX_STRING_LENGTH,stdin);

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", chArray_SSP);

    // *** STRING LENGTH ***
    printf("\n\n");
    iStringLength_SSP = MyStrlen(chArray_SSP);
    printf("Length Of String Is = %d Characters !!!\n\n", iStringLength_SSP);
    if (chArray_SSP)
    {
        free(chArray_SSP);
        chArray_SSP = NULL;
    }
    return(0);
}

int MyStrlen(char *str)
{
    //variable declarations
    int j_SSP;
    int string_length_SSP = 0;

    //code
    // *** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRSTOCCURENCE OF NULL-TERMINATING CHARACTER ( \0 ) ***
    for (j_SSP = 0; j_SSP < MAX_STRING_LENGTH; j_SSP++) 
    {
        if (*(str + j_SSP) == '\0')
            break;
        else
            string_length_SSP++;
    }
    return(string_length_SSP);
}
