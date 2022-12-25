#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype
    void MyStrcpy(char *, char *);
    int MyStrlen(char *);
  
    //variable declarations 
    char *chArray_Original_SSP = NULL, *chArray_Copy_SSP = NULL; // A Character Array Is A String 
    int original_string_length_SSP;
  
    //code
    // *** STRING INPUT ***
    printf("\n\n");
    chArray_Original_SSP = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray_Original_SSP == NULL)
    {
        printf("MEMORY ALLOCATION FOR ORIGINAL STRING FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
    printf("Enter A String : \n\n");
    fgets(chArray_Original_SSP, MAX_STRING_LENGTH,stdin);
  
    original_string_length_SSP = MyStrlen(chArray_Original_SSP);
    chArray_Copy_SSP = (char *)malloc(original_string_length_SSP * sizeof(char));
    if (chArray_Copy_SSP == NULL)
    {
        printf("MEMORY ALLOCATION FOR COPIED STRING FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
  
    // *** STRING COPY ***
    MyStrcpy(chArray_Copy_SSP, chArray_Original_SSP);
  
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String Entered By You (i.e : 'chArray_Original') Is : \n\n");
    printf("%s\n", chArray_Original_SSP);
    printf("\n\n");
    printf("The Copied String (i.e : 'chArray_Copy') Is : \n\n");
    printf("%s\n", chArray_Copy_SSP);
    if (chArray_Copy_SSP)
    {
        free(chArray_Copy_SSP);
        chArray_Copy_SSP = NULL;
        printf("\n\n");
        printf("MEMORY ALLOCATED FOR COPIED STRING HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    if (chArray_Original_SSP)
    {
        free(chArray_Original_SSP);
        chArray_Original_SSP = NULL;
        printf("\n\n");
        printf("MEMORY ALLOCATED FOR ORIGINAL STRING HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    return(0);
}

void MyStrcpy(char *str_destination, char *str_source)
{
    //function prototype
    int MyStrlen(char *);
  
    //variable declarations 
    int iStringLength = 0;
    int j;
  
    //code
    iStringLength = MyStrlen(str_source);
    for (j = 0; j < iStringLength; j++)
        *(str_destination + j) = *(str_source + j);
    *(str_destination + j) = '\0';
}

int MyStrlen(char *str)
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