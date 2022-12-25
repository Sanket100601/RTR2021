#include <stdio.h>
#define NUM_ELEMENTS 10
int main(void)
{
    //variable declarations 
    int iArray_SSP[NUM_ELEMENTS];
    int i_SSP, num_SSP, sum_SSP = 0;
    //code
    printf("\n\n");
    printf("Enter Integer Elememts For Array : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ELEMENTS; i_SSP++)
    {
        scanf("%d", &num_SSP);
        iArray_SSP[i_SSP] = num_SSP;
    }
    for (i_SSP = 0; i_SSP < NUM_ELEMENTS; i_SSP++)
    {
        sum_SSP = sum_SSP + iArray_SSP[i_SSP];
    }
    printf("\n\n");
    printf("Sum Of ALL Elements Of Array = %d\n\n", sum_SSP);
    return(0);
}
/*
Enter Integer Elememts For Array :

1
2
3
4 
5

6
7
8
98
9


Sum Of ALL Elements Of Array = 143
*/