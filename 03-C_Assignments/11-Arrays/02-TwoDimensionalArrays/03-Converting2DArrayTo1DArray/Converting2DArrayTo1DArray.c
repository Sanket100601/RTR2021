#include <stdio.h>
#define NUM_ROWS 5
#define NUM_COLUMNS 3
int main(void)
{
    //variable declaraions
    int iArray_2D_SSP[NUM_ROWS][NUM_COLUMNS]; // TOTAL NUMBER OF ELEMENTS = NUM_ROWS * NUM_COLUMNS
    int iArray_1D_SSP[NUM_ROWS * NUM_COLUMNS];
    int i_SSP, j_SSP;
    int num_SSP;
    //code
    printf("Enter Elements Of Your Choice To Fill Up The Integer 2D Array : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        printf("For ROW NUMBER %d : \n", (i_SSP + 1));
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
        {
            printf("Enter Element Number %d : \n", (j_SSP + 1));
            scanf("%d", &num_SSP);
            iArray_2D_SSP[i_SSP][j_SSP] = num_SSP;
        }
        printf("\n\n");
    }
    
    // *** DISPLAY OF 2D ARRAY ***
    printf("\n\n");
    printf("Two-Dimensional ( 2D ) Array Of Integers : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        printf("****** ROW %d ******\n", (i_SSP + 1));
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
        {
            printf("iArray_2D[%d][%d] = %d\n", i_SSP, j_SSP, iArray_2D_SSP[i_SSP][j_SSP]);
        }
        printf("\n\n");
    }
    
    // *** CONVERTING 2D INTEGER ARRAY TO 1D INTEGER ARRAY ***
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
        {
        iArray_1D_SSP[(i_SSP * NUM_COLUMNS) + j_SSP] = iArray_2D_SSP[i_SSP][j_SSP];
        }
    }
    
    // *** PRINTING 1D ARRAY ***
    printf("\n\n");
    printf("One-Dimensional ( 1D ) Array Of Integers : \n\n");
    for (i_SSP = 0; i_SSP < (NUM_ROWS * NUM_COLUMNS); i_SSP++)
    {
        printf("iArray_1D[%d] = %d\n", i_SSP, iArray_1D_SSP[i_SSP]);
    }
    printf("\n\n");
    return(0);
}
/*
For ROW NUMBER 1 : 
Enter Element Number 1 :
1
Enter Element Number 2 : 
2
Enter Element Number 3 : 
3


For ROW NUMBER 2 :
Enter Element Number 1 :
4
Enter Element Number 2 : 
5
Enter Element Number 3 : 
6


For ROW NUMBER 3 :
Enter Element Number 1 :
7
Enter Element Number 2 : 
8
Enter Element Number 3 : 
9


For ROW NUMBER 4 :
Enter Element Number 1 :
0
Enter Element Number 2 : 
1
Enter Element Number 3 : 
2


For ROW NUMBER 5 :
Enter Element Number 1 :
3
Enter Element Number 2 : 
4
Enter Element Number 3 : 
5



iArray_1D[6] = 7
iArray_1D[7] = 8
iArray_1D[8] = 9
iArray_1D[9] = 0
iArray_1D[10] = 1
iArray_1D[11] = 2
iArray_1D[12] = 3
iArray_1D[13] = 4
iArray_1D[14] = 5
*/
