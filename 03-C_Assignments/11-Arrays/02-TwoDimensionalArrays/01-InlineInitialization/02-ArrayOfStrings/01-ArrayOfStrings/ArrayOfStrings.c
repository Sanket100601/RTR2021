#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype
    int MyStrlen(char[]);
    //variable declaraions
    // *** A 'STRING' IS AN ARRAY OF CHARACTERS ... so char[] IS A char ARRAY AND HENCE, char[] IS A 'STRING' ***
    // *** AN ARRAY OF char ARRAYS IS AN ARRAY OF STRINGS !!! ***
    // *** HENCE, char[] IS ONE char ARRAY AND HENCE, IS ONE STRING ***
    // *** HENCE, char[][] IS AN ARRAY OF char ARRAYS AND HENCE, IS AN ARRAY OF STRINGS ***
    //Here, the string array can allow a maximum number of 10 strings (10 rows) and each of these 10 strings can have only upto 15 characters maximum (15 columns)
    char strArray_SSP[10][15] = { "Hello!", "Welcome", "To", "Real", "Time", "Rendering", "Batch", "(2020-21)", "Of", "ASTROMEDICOMP." }; //IN-LINE INITIALIZATION
    int char_size_SSP;
    int strArray_size_SSP;
    int strArray_num_elements_SSP, strArray_num_rows_SSP, strArray_num_columns_SSP;
    int strActual_num_chars_SSP = 0;
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
    for (i_SSP = 0; i_SSP < strArray_num_rows_SSP; i_SSP++)
    {
        strActual_num_chars_SSP = strActual_num_chars_SSP + MyStrlen(strArray_SSP[i_SSP]);
    }
    printf("Actual Number of Elements (Characters) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", strActual_num_chars_SSP);
    printf("\n\n");
    printf("Strings In The 2D Array : \n\n");
    //Since, char[][] is an array of strings, referencing only by the row number (first []) will give the row or the string
    //The Column Number (second []) is the particular character in that string / row
    printf("%s ", strArray_SSP[0]);
    printf("%s ", strArray_SSP[1]);
    printf("%s ", strArray_SSP[2]); 
    printf("%s ", strArray_SSP[3]);
    printf("%s ", strArray_SSP[4]);
    printf("%s ", strArray_SSP[5]);
    printf("%s ", strArray_SSP[6]);
    printf("%s ", strArray_SSP[7]);
    printf("%s ", strArray_SSP[8]);
    printf("%s\n\n", strArray_SSP[9]);
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
Size Of Two Dimensional ( 2D ) Character Array (String Array) Is = 150

Number of Rows (Strings) In Two Dimensional ( 2D ) Character Array (String Array) Is = 10

Number of Columns In Two Dimensional ( 2D ) Character Array (String Array) Is = 15

Maximum Number of Elements (Characters) In Two Dimensional ( 2D ) Character Array (String Array) Is = 150

Actual Number of Elements (Characters) In Two Dimensional ( 2D ) Character Array (String Array) Is = 62



Strings In The 2D Array :

Hello! Welcome To Real Time Rendering Batch (2020-21) Of ASTROMEDICOMP.
*/

