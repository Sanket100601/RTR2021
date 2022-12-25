#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3
int main(void)
{
    //variable declarations
    int iArray_SSP[NUM_ROWS][NUM_COLUMNS];
    int i_SSP, j_SSP;
    
    //code
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
            iArray_SSP[i_SSP][j_SSP] = (i_SSP + 1) * (j_SSP + 1);
    }
    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
        {
            printf("iArray[%d][%d] = %d \t \t At Address : %p\n", i_SSP, j_SSP, iArray_SSP[i_SSP][j_SSP], &iArray_SSP[i_SSP][j_SSP]);
        }
        printf("\n\n");
    }
    return(0);
}