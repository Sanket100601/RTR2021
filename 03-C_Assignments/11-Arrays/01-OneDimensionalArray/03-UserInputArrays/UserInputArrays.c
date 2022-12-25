#include <stdio.h>
// MACRO CONSTANT USED AS ARRAY SIZE IN SUBSCRIPT AND AS ARRAY LENGTH.
// HENCE, THIS PROGRAM'S ARRAYS' SIZES CAN BE SIMPLY CHANGED BY CHANGING THESE 
//FOLLOWING 3 GLOBAL MACRO CONSTANT VALUES, BEFORE COMPILING, LINKING AND 
//EXECUTING THE PROGRAM !!!
#define INT_ARRAY_NUM_ELEMENTS 5
#define FLOAT_ARRAY_NUM_ELEMENTS 3
#define CHAR_ARRAY_NUM_ELEMENTS 15
int main(void)
{
    //variable declarations 
    int iArray_SSP[INT_ARRAY_NUM_ELEMENTS];
    float fArray_SSP[FLOAT_ARRAY_NUM_ELEMENTS];
    char cArray_SSP[CHAR_ARRAY_NUM_ELEMENTS];
    int i_SSP, num_SSP;
    //code
    // ********** ARRAY ELEMENTS INPUT **********
    printf("\n\n");
    printf("Enter Elements For 'Integer' Array : \n");
    for (i_SSP = 0; i_SSP < INT_ARRAY_NUM_ELEMENTS; i_SSP++)
    scanf("%d", &iArray_SSP[i_SSP]);
    printf("\n\n");
    printf("Enter Elements For 'Floating-Point' Array : \n");
    for (i_SSP = 0; i_SSP < FLOAT_ARRAY_NUM_ELEMENTS; i_SSP++)
    scanf("%f", &fArray_SSP[i_SSP]);
    printf("\n\n");
    printf("Enter Elements For 'Character' Array : \n");
    for (i_SSP = 0; i_SSP < CHAR_ARRAY_NUM_ELEMENTS; i_SSP++)
    {
        cArray_SSP[i_SSP] = getch();
        printf("%c\n", cArray_SSP[i_SSP]);  
    }   
    // ********** ARRAY ELEMENTS OUTPUT **********
    printf("\n\n"); 
    printf("Integer Array Entered By You : \n\n");
    for (i_SSP = 0; i_SSP < INT_ARRAY_NUM_ELEMENTS; i_SSP++)
    printf("%d\n", iArray_SSP[i_SSP]);
    printf("\n\n");
    printf("Floating-Point Array Entered By You : \n\n");
    for (i_SSP = 0; i_SSP < FLOAT_ARRAY_NUM_ELEMENTS; i_SSP++)
    printf("%f\n", fArray_SSP[i_SSP]);
    printf("\n\n");
    printf("Character Array Entered By You : \n\n");
    for (i_SSP = 0; i_SSP < CHAR_ARRAY_NUM_ELEMENTS; i_SSP++)
    printf("%c\n", cArray_SSP[i_SSP]);
    return(0);
}
/*
Enter Elements For 'Integer' Array : 
1
2
3
4
5


Enter Elements For 'Floating-Point' Array : 
1.1
1.2
1.3


Enter Elements For 'Character' Array : 
c
h
a
r
a
c
t
e
r
a
r
r
a
y
s


Integer Array Entered By You : 

1
2
3
4
5


Floating-Point Array Entered By You :

1.100000
1.200000
1.300000


Character Array Entered By You :

c
h
a
r
a
c
t
e
r
a
r
r
a
y
s
*\