#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 5

int main(void)
{
    //variable declarations
    int *iArray_SSP[NUM_ROWS]; //A 2D Array which will have 5 rows and number of columns can be decided later on ... 
    int i_SSP, j_SSP;

    //code
    printf("\n\n");
    printf("********** MEMORY ALLOCATION TO 2D INTEGER ARRAY **********\n\n");
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        //ROW 0 WILL HAVE (NUM_COLUMNS - 0) = (5 - 0) = 5 COLUMNS...
        //ROW 1 WILL HAVE (NUM_COLUMNS - 1) = (5 - 1) = 4 COLUMNS...
        //ROW 2 WILL HAVE (NUM_COLUMNS - 2) = (5 - 2) = 3 COLUMNS...
        //ROW 3 WILL HAVE (NUM_COLUMNS - 3) = (5 - 3) = 2 COLUMNS...
        //ROW 4 WILL HAVE (NUM_COLUMNS - 4) = (5 - 4) = 1 COLUMN...
        //BEACUSE OF THIS, THERE IS NO CONTIGUOUS MEMORY ALLOCATION ... HENCE, ALTHOUGH WE MAY USE THE DATA AS A 2D ARRAY, IT IS NOT REALLY A 2D ARRAY IN MEMORY ...
        
        iArray_SSP[i_SSP] = (int *)malloc((NUM_COLUMNS - i_SSP) * sizeof(int));
        if (iArray_SSP[i_SSP] == NULL)
        {
            printf("FAILED TO ALLOCATE MEMORY TO ROW %d OF 2D INTEGER ARRAY !!! EXITTING NOW...\n\n", i_SSP);
            exit(0);
        }
        else
            printf("MEMORY ALLOCATION TO ROW %d OF 2D INTEGER ARRAY SUCCEEDED !!!\n\n", i_SSP);
    }
    for (i_SSP = 0; i_SSP < 5; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < (NUM_COLUMNS - i_SSP); j_SSP++)
        {
            iArray_SSP[i_SSP][j_SSP] = (i_SSP * 1) + (j_SSP * 1);
        }
    }
    for (i_SSP = 0; i_SSP < 5; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < (NUM_COLUMNS - i_SSP); j_SSP++)
        {
            printf("iArray[%d][%d] = %d \t At Address : %p\n", i_SSP, j_SSP, iArray_SSP[i_SSP][j_SSP], &iArray_SSP[i_SSP][j_SSP]);
        }   
        printf("\n");
    }
    for (i_SSP = (NUM_ROWS - 1); i_SSP >= 0; i_SSP--)
    {
        if (iArray_SSP[i_SSP])
        {
            free(iArray_SSP[i_SSP]);
            iArray_SSP[i_SSP] = NULL;
            printf("MEMORY ALLOCATED TO ROW %d HAS BEEN SUCCESSFULLY FREED !!!\n\n", i_SSP);
        }
    }
    return(0);
}