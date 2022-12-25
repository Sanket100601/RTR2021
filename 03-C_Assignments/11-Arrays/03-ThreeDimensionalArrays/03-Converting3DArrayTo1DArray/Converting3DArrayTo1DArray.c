#include <stdio.h>
#define NUM_ROWS 5
#define NUM_COLUMNS 3
#define DEPTH 2
int main(void)
{
    //variable declaraions
    //IN-LINE INITIALIZATION
    int iArray_SSP[NUM_ROWS][NUM_COLUMNS][DEPTH] = { { { 9, 18 }, { 27, 36 }, { 45, 54 } },
                                                     { { 8, 16 }, { 24, 32 }, { 40, 48 } },
                                                     { { 7, 14 }, { 21, 28 }, { 35, 42 } }, 
                                                     { { 6, 12 }, { 18, 24 }, { 30, 36 } },
                                                     { { 5, 10 }, { 15, 20 }, { 25, 30 } } };
    int i_SSP, j_SSP, k_SSP;
    int iArray_1D_SSP[NUM_ROWS * NUM_COLUMNS * DEPTH]; // 5 * 3 * 2 ELEMENTS => 30 ELEMENTS IN 1D ARRAY

    //code
    // ****** DISPLAY 3D ARRAY ******
    printf("\n\n");
    printf("Elements In The 3D Array : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {   
        printf("****** ROW %d ******\n", (i_SSP + 1));
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
        {
            printf("****** COLUMN %d ******\n", (j_SSP + 1));
            for (k_SSP = 0; k_SSP < DEPTH; k_SSP++)
            {
                printf("iArray[%d][%d][%d] = %d\n", i_SSP, j_SSP, k_SSP, iArray_SSP[i_SSP][j_SSP][k_SSP]);
            }
            printf("\n");
        }
        printf("\n");
    }
    // ****** CONVERTING 3D TO 1D ******
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)   
        {
            for (k_SSP = 0; k_SSP < DEPTH; k_SSP++)
            {
                iArray_1D_SSP[(i_SSP * NUM_COLUMNS * DEPTH) + (j_SSP * DEPTH) + k_SSP] = iArray_SSP[i_SSP][j_SSP][k_SSP];
            }
        }
    }
    // ****** DISPLAY 1D ARRAY ******
    printf("\n\n\n\n");
    printf("Elements In The 1D Array : \n\n");
    for (i_SSP = 0; i_SSP < (NUM_ROWS * NUM_COLUMNS * DEPTH); i_SSP++)
    {
        printf("iArray_1D[%d] = %d\n", i_SSP, iArray_1D_SSP[i_SSP]);
    }
    return(0);
}
/*
Elements In The 3D Array : 

****** ROW 1 ******
****** COLUMN 1 ******
iArray[0][0][0] = 9
iArray[0][0][1] = 18

****** COLUMN 2 ******
iArray[0][1][0] = 27
iArray[0][1][1] = 36

****** COLUMN 3 ******
iArray[0][2][0] = 45
iArray[0][2][1] = 54


****** ROW 2 ******
****** COLUMN 1 ******
iArray[1][0][0] = 8
iArray[1][0][1] = 16

****** COLUMN 2 ******
iArray[1][1][0] = 24
iArray[1][1][1] = 32

****** COLUMN 3 ******
iArray[1][2][0] = 40
iArray[1][2][1] = 48


****** ROW 3 ******
****** COLUMN 1 ******
iArray[2][0][0] = 7
iArray[2][0][1] = 14

****** COLUMN 2 ******
iArray[2][1][0] = 21
iArray[2][1][1] = 28

****** COLUMN 3 ******
iArray[2][2][0] = 35
iArray[2][2][1] = 42


****** ROW 4 ******
****** COLUMN 1 ******
iArray[3][0][0] = 6
iArray[3][0][1] = 12

****** COLUMN 2 ******
iArray[3][1][0] = 18
iArray[3][1][1] = 24

****** COLUMN 3 ******
iArray[3][2][0] = 30
iArray[3][2][1] = 36


****** ROW 5 ******
****** COLUMN 1 ******
iArray[4][0][0] = 5
iArray[4][0][1] = 10

****** COLUMN 2 ******
iArray[4][1][0] = 15
iArray[4][1][1] = 20

****** COLUMN 3 ******
iArray[4][2][0] = 25
iArray[4][2][1] = 30






Elements In The 1D Array :

iArray_1D[0] = 9
iArray_1D[1] = 18
iArray_1D[2] = 27
iArray_1D[3] = 36
iArray_1D[4] = 45
iArray_1D[5] = 54
iArray_1D[6] = 8
iArray_1D[7] = 16
iArray_1D[8] = 24
iArray_1D[9] = 32
iArray_1D[10] = 40
iArray_1D[11] = 48
iArray_1D[12] = 7
iArray_1D[13] = 14
iArray_1D[14] = 21
iArray_1D[15] = 28
iArray_1D[16] = 35
iArray_1D[17] = 42
iArray_1D[18] = 6
iArray_1D[19] = 12
iArray_1D[20] = 18
iArray_1D[21] = 24
iArray_1D[22] = 30
iArray_1D[23] = 36
iArray_1D[24] = 5
iArray_1D[25] = 10
iArray_1D[26] = 15
iArray_1D[27] = 20
iArray_1D[28] = 25
iArray_1D[29] = 30
*/