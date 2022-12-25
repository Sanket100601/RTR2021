#include <stdio.h>
#include <stdlib.h>

int main(void)
{
//variable declarations
    int **ptr_iArray_SSP = NULL; //A pointer-to-pointer to integer ... but can alsohold base address of a 2D Array which will can have any number of rows and any number of columns ... 
    int i_SSP, j_SSP;
    int num_rows_SSP, num_columns_SSP;

    //code
    // *** ACCEPT NUMBER OF ROWS 'num_rows' FROM USER ***
    printf("\n\n");
    printf("Enter Number Of Rows : ");
    scanf("%d", &num_rows_SSP);

    // *** ACCEPT NUMBER OF COLUMNS 'num_columns' FROM USER ***
    printf("\n\n");
    printf("Enter Number Of Columns : ");
    scanf("%d", &num_columns_SSP);

    // *** ALLOCATING MEMORY TO 1D ARRAY CONSISTING OF BASE ADDRESS OF ROWS ***
    printf("\n\n");
    printf("********** MEMORY ALLOCATION TO 2D INTEGER ARRAY **********\n\n");  
    ptr_iArray_SSP = (int **)malloc(num_rows_SSP * sizeof(int *));
    if (ptr_iArray_SSP == NULL)
    {
        printf("FAILED TO ALLOCATE MEMORY TO %d ROWS OF 2D INTEGER ARRAY !!! EXITTING NOW...\n\n", num_rows_SSP);
        exit(0);
    }
    else
    printf("MEMORY ALLOCATION TO %d ROWS OF 2D INTEGER ARRAY SUCCEEDED !!!\n\n", num_rows_SSP);
    
    // *** ALLOCATING MEMORY TO EACH ROW WHICH IS A 1D ARRAY CONTAINING CONSISTING OF COLUMNS WHICH CONTAIN THE ACTUAL INTEGERS ***
    for (i_SSP = 0; i_SSP < num_rows_SSP; i_SSP++)
    {
        ptr_iArray_SSP[i_SSP] = (int *)malloc(num_columns_SSP * sizeof(int)); //ALLOCATING MEMORY (Number Of Columns * size of 'int') TO ROW 'i' 
        if (ptr_iArray_SSP[i_SSP] == NULL) //ROW 'i' MEMORY ALLOCATED ?
        {
            printf("FAILED TO ALLOCATE MEMORY TO COLUMNS OF ROW %d OF 2D INTEGER ARRAY !!! EXITTING NOW...\n\n", i_SSP);
            exit(0);
        }
        else
            printf("MEMORY ALLOCATION TO COLUMNS OF ROW %d OF 2D INTEGER ARRAY SUCCEEDED !!!\n\n", i_SSP);
    }
    
    // *** FILLING UP VALUES ***
    for (i_SSP = 0; i_SSP < num_rows_SSP; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < num_columns_SSP; j_SSP++)
        {
            ptr_iArray_SSP[i_SSP][j_SSP] = (i_SSP * 1) + (j_SSP * 1); // can also use : *(*(ptr_iArray + i) + j) = (i * 1) + (j * 1)
        }
    }
    // *** DISPLAYING VALUES ***
    for (i_SSP = 0; i_SSP < num_rows_SSP; i_SSP++)
    {
        printf("Base Address Of Row %d : ptr_iArray[%d] = %p \t At Address : %p\n", i_SSP, i_SSP, ptr_iArray_SSP[i_SSP], &ptr_iArray_SSP[i_SSP]);
    }
    printf("\n\n");
    for (i_SSP = 0; i_SSP < num_rows_SSP; i_SSP++)
    {   
        for (j_SSP = 0; j_SSP < num_columns_SSP; j_SSP++)
        {
            printf("ptr_iArray[%d][%d] = %d \t At Address : %p\n", i_SSP, j_SSP, 
            ptr_iArray_SSP[i_SSP][j_SSP], &ptr_iArray_SSP[i_SSP][j_SSP]); // can also use *(*(ptr_iArray + i) + j) for value and *(ptr_iArray + i) + j for address ...
        }
        printf("\n");
    }
    // *** FREEING MEMORY ALLOCATED TO EACH ROW ***
    for (i_SSP = (num_rows_SSP - 1); i_SSP >= 0; i_SSP--)
    {
        if (ptr_iArray_SSP[i_SSP])  
        {
            free(ptr_iArray_SSP[i_SSP]);
            ptr_iArray_SSP[i_SSP] = NULL;
            printf("MEMORY ALLOCATED TO ROW %d HAS BEEN SUCCESSFULLY FREED !!!\n\n", i_SSP);
        }
    }
    // *** FREEING MEMORY ALLOCATED TO 1D ARRAY CONSISTING OF BASE ADDRESSES OF ROWS ***
    if (ptr_iArray_SSP)
    {
        free(ptr_iArray_SSP);
        ptr_iArray_SSP = NULL;
        printf("MEMORY ALLOCATED TO ptr_iArray HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    return(0);
}
