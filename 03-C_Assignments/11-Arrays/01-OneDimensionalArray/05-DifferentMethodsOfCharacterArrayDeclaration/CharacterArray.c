#include <stdio.h>
int main(void)
{
    //variable declarations 
    char chArray_01[] = { 'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P', '\0' }; // Must give \0 Explicitly For Proper Initialization
    char chArray_02[9] = { 'W', 'E', 'L', 'C', 'O', 'M', 'E', 'S', '\0' }; // Must give \0 Explicitly For Proper Initialization
    char chArray_03[] = { 'Y', 'O', 'U', '\0' }; // Must give \0 Explicitly For Proper Initialization
    char chArray_04[] = "To"; // \0 is assumed, size is given as 3, although string has only 2 characters
    char chArray_05[] = "REAL TIME RENDERING BATCH OF 2020-21"; // \0 is assumed, size is given as 40, although string has 39 characters
    char chArray_WithoutNullTerminator[] = { 'H', 'e', 'l', 'l', 'o' };
    //code
    printf("\n\n");
    printf("Size Of chArray_01 : %lu\n\n", sizeof(chArray_01));
    printf("Size Of chArray_02 : %lu\n\n", sizeof(chArray_02));
    printf("Size Of chArray_03 : %lu\n\n", sizeof(chArray_03));
    printf("Size Of chArray_04 : %lu\n\n", sizeof(chArray_04));
    printf("Size Of chArray_05 : %lu\n\n", sizeof(chArray_05));
    printf("\n\n");
    printf("The Strings Are : \n\n");
    printf("chArray_01 : %s\n\n", chArray_01);
    printf("chArray_02 : %s\n\n", chArray_02);
    printf("chArray_03 : %s\n\n", chArray_03);
    printf("chArray_04 : %s\n\n", chArray_04);
    printf("chArray_05 : %s\n\n", chArray_05);
    printf("\n\n");
    printf("Size Of chArray_WithoutNullTerminator : %lu\n\n", sizeof(chArray_WithoutNullTerminator));
    printf("chArray_WithoutNullTerminator : %s\n\n", chArray_WithoutNullTerminator); //Will display garbage value at the end of string due to absence of \0
    return(0);
}
/*
Size Of chArray_01 : 14

Size Of chArray_02 : 9

Size Of chArray_03 : 4

Size Of chArray_04 : 3

Size Of chArray_05 : 37



The Strings Are :

chArray_01 : ASTROMEDICOMP

chArray_02 : WELCOMES

chArray_03 : YOU

chArray_04 : To

chArray_05 : REAL TIME RENDERING BATCH OF 2020-21



Size Of chArray_WithoutNullTerminator : 5

chArray_WithoutNullTerminator : HelloREAL TIME RENDERING BATCH OF 2020-21

*/
