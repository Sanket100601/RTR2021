#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3
int main(void)
{
    //variable declarations
    int i_SSP, j_SSP;
    int **ptr_iArray_SSP = NULL;
    //code
    // *** EVERY ROW OF A 2D ARRAY IS AN INTEGER ARRAY ITSELF COMPRISING OF 'NUM_COLUMNS' INTEGER ELEMENTS ***
    // *** THERE ARE 5 ROWS AND 3 COLUMNS IN A 2D INTEGER ARRAY. EACH OF THE 5 ROWS IS A 1D ARRAY OF 3 INTEGERS. 
    // *** HENCE, EACH OF THESE 5 ROWS THEMSELVES BEING ARRAYS, WILL BE THE BASE ADDRESSES OF THEIR RESPECTIVE ROWS ***
    
    printf("\n\n");
    
    // *** MEMORY ALLOCATION ***
    ptr_iArray_SSP = (int **)malloc(NUM_ROWS * sizeof(int *)); //ptr_iArray is the name and base address of 1D Array containing 5 integer pointers to 5 integer arrays ... so it is an array containing elelments of data type (int *)
  
    if (ptr_iArray_SSP == NULL)
    {
        printf("MEMORY ALLOCATION TO THE 1D ARRAY OF BASE ADDRESSES OF %d ROWS FAILED !!! EXITTING NOW...\n\n", NUM_ROWS);
        exit(0);
    }
    else
        printf("MEMORY ALLOCATION TO THE 1D ARRAY OF BASE ADDRESSES OF %d ROWS HAS SUCCEEDED !!!\n\n", NUM_ROWS);
    
    // *** ALLOCATING MEMORY TO EACH ROW ***
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        ptr_iArray_SSP[i_SSP] = (int *)malloc(NUM_COLUMNS * sizeof(int)); //ptr_iArray[i] is the base address of ith row ...
        if (ptr_iArray_SSP == NULL)
        {
            printf("MEMORY ALLOCATION TO THE COLUMNS OF ROW %d FAILED !!! EXITTING NOW...\n\n", i_SSP);
            exit(0);
        }
        else
            printf("MEMORY ALLOCATION TO THE COLUMNS OF ROW %d HAS SUCCEEDED !!!\n\n", i_SSP);
    }
  // *** ASSIGNING VALUES ***
  for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
  {
    for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
    {
        *(*(ptr_iArray_SSP + i_SSP) + j_SSP) = (i_SSP + 1) * (j_SSP + 1); // ptr_iArray[i][j] =(i + 1) * (j + 1);
    }
  }
  // *** DISPLAYING VALUES ***
  printf("\n\n");
  printf("2D Integer Array Elements Along With Addresses : \n\n");
  for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
  {
    for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
    {
      printf("ptr_iArray_Row[%d][%d] = %d \t \t At Address &ptr_iArray_Row[%d][%d] : %p\n", i_SSP, j_SSP, ptr_iArray_SSP[i_SSP][j_SSP], i_SSP, j_SSP, &ptr_iArray_SSP[i_SSP][j_SSP]);
    }
    printf("\n\n");
  }
  // *** FREEING ALLOCATED MEMORY ***
  // *** FREEING MEMORY OF EACH ROW ***
  for (i_SSP = (NUM_ROWS - 1); i_SSP >= 0; i_SSP--)
  {
    if (*(ptr_iArray_SSP + i_SSP)) // if(ptr_iArray[i])
    {
      free(*(ptr_iArray_SSP + i_SSP)); // free(ptr_iArray[i])
      *(ptr_iArray_SSP + i_SSP) = NULL; // ptr_iArray[i] = NULL;
      printf("MEMORY ALLOCATED TO ROW %d HAS BEEN SUCCESSFULLY FREED !!!\n\n", i_SSP);
    }
  }
  // *** FREEING MEMORY OF ptr_iArray WHICH IS THE ARRAY OF 5 INTEGER POINTERS ... THAT IT, IT IS AN ARRAY HAVING 5 INTEGER ADDRESSES (TYPE int *) ***
  if (ptr_iArray_SSP)
  {
    free(ptr_iArray_SSP);
    ptr_iArray_SSP = NULL;
    printf("MEMORY ALLOCATED TO ptr_iArray HAS BEEN SUCCESSFULLY FREED !!!\n\n");
  }
  return(0);
}