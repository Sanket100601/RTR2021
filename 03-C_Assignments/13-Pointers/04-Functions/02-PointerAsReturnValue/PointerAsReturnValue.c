#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512
int main(void)
{
    //function declarations
    char* ReplaceVowelsWithHashSymbol(char *);

    //variable declaration
    char string[MAX_STRING_LENGTH];
    char *replaced_string = NULL;

    //code
    printf("\n\n");
    printf("Enter String : ");
    fgets(string, MAX_STRING_LENGTH,stdin);
    replaced_string = ReplaceVowelsWithHashSymbol(string);
    if (replaced_string == NULL)
    {
        printf("ReplaceVowelsWithHashSymbol() Function Has Failed !!! Exitiing Now...\n\n");
        exit(0);
    }
    printf("\n\n");
    printf("Replaced String Is : \n\n");
    printf("%s\n\n", replaced_string);
    if (replaced_string)
    {
        free(replaced_string);
        replaced_string = NULL;
    }
    return(0);
}

char* ReplaceVowelsWithHashSymbol(char *s)
{
    //function prototype
    void MyStrcpy(char *, char *);
    int MyStrlen(char *);

    //varibale declarations
    char *new_string = NULL;
    int i;

    //code
    new_string = (char *)malloc(MyStrlen(s) * sizeof(char));
    if (new_string == NULL) 
    {
        printf("COULD NOT ALLOCATE MEMORY FOR NEW STRING !!!\n\n");
    return(NULL);
    }
    MyStrcpy(new_string, s);
    for (i = 0; i < MyStrlen(new_string); i++)
    {
        switch (new_string[i])  
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
            new_string[i] = '#';
            break;
            default:
                break;
        }
    }
    return(new_string);
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
