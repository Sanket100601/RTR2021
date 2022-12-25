#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
  //variable declarations
  int *iArray_SSP[NUM_ROWS]; //A 2D Array which will have 5 rows and number of columns can be decided later on ... 
  int i_SSP, j_SSP;
 
  //code
  printf("\n\n");
  for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        iArray_SSP[i_SSP] = (int *)malloc(NUM_COLUMNS * sizeof(int));
        if (iArray_SSP[i_SSP] == NULL)
        {
            printf("FAILED TO ALLOCATE MEMORY TO ROW %d OF 2D INTEGER ARRAY !!! EXITTING NOW...\n\n", i_SSP);
            exit(0);
        }
        else
            printf("MEMORY ALLOCATION TO ROW %d OF 2D INTEGER ARRAY SUCCEEDED !!!\n\n", i_SSP);
    }
    //ASSIGNING VALUES TO 2D ARRAY ...
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
        {
            iArray_SSP[i_SSP][i_SSP] = (i_SSP + 1) * (j_SSP + 1);
        }
    }
 
    //DISPLAYING 2D ARRAY ...
    printf("\n\n");
    printf("DISPLAYING 2D ARRAY : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
        {
            printf("iArray[%d][%d] = %d\n", i_SSP, j_SSP, iArray_SSP[i_SSP][j_SSP]);
        }
        printf("\n\n");
    }
    printf("\n\n");
    //FREEING MEMORY ASSIGNED TO 2D ARRAY (MUST BE DONE IN REVERSE ORDER)
    for (i_SSP = (NUM_ROWS - 1); i_SSP >= 0; i_SSP--)
    {
        free(iArray_SSP[i_SSP]);
        iArray_SSP[i_SSP] = NULL;
        printf("MEMORY ALLOCATED TO ROW %d Of 2D INTEGER ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n", i_SSP);
    }
    return(0);
}