#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype
    void MyStrcpy(char[], char[]);
    //variable declaraions
    // *** A 'STRING' IS AN ARRAY OF CHARACTERS ... so char[] IS A char ARRAY AND HENCE, char[] IS A 'STRING' ***
    // *** AN ARRAY OF char ARRAYS IS AN ARRAY OF STRINGS !!! ***
    // *** HENCE, char[] IS ONE char ARRAY AND HENCE, IS ONE STRING ***
    // *** HENCE, char[][] IS AN ARRAY OF char ARRAYS AND HENCE, IS AN ARRAY OF STRINGS ***
    //Here, the string array can allow a maximum number of 5 strings (5 rows) and each of these 5 strings can have only upto 10 characters maximum (10 columns)
    char strArray_SSP[5][10]; // 5 ROWS (0, 1, 2, 3, 4) -> 5 STRINGS (EACH STRING CAN HAVE A MAXIMUM OF 10 CHARACTERS)
    int char_size_SSP;
    int strArray_size_SSP;
    int strArray_num_elements_SSP, strArray_num_rows_SSP, strArray_num_columns_SSP;
    int i_SSP;
    //code
    printf("\n\n");
    char_size_SSP = sizeof(char);
    strArray_size_SSP = sizeof(strArray_SSP);
    printf("Size Of Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", strArray_size_SSP);
    strArray_num_rows_SSP = strArray_size_SSP / sizeof(strArray_SSP[0]);
    printf("Number of Rows (Strings) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", strArray_num_rows_SSP);
    strArray_num_columns_SSP = sizeof(strArray_SSP[0]) / char_size_SSP;
    printf("Number of Columns In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", strArray_num_columns_SSP);
    strArray_num_elements_SSP = strArray_num_rows_SSP * strArray_num_columns_SSP;
    printf("Maximum Number of Elements (Characters) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", strArray_num_elements_SSP);
    // *** PIECE-MEAL ASSIGNMENT ***
    MyStrcpy(strArray_SSP[0], "My");
    MyStrcpy(strArray_SSP[1], "Name");
    MyStrcpy(strArray_SSP[2], "Is");
    MyStrcpy(strArray_SSP[3], "Sanket");
    MyStrcpy(strArray_SSP[4], "Pawar");
    printf("\n\n");
    printf("The Strings In the 2D Character Array Are : \n\n");
    for (i_SSP = 0; i_SSP < strArray_num_rows_SSP; i_SSP++)
    printf("%s ", strArray_SSP[i_SSP]);
    printf("\n\n");
    return(0);
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
Size Of Two Dimensional ( 2D ) Character Array (String Array) Is = 50

Number of Rows (Strings) In Two Dimensional ( 2D ) Character Array (String Array) Is = 5

Number of Columns In Two Dimensional ( 2D ) Character Array (String Array) Is = 10

Maximum Number of Elements (Characters) In Two Dimensional ( 2D ) Character Array (String Array) Is = 50



The Strings In the 2D Character Array Are :

My Name Is Sanket Pawar
*/