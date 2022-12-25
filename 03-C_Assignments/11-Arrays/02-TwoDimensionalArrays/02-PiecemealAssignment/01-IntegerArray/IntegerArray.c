#include <stdio.h>
int main(void)
{
    //variable declaraions
    int iArray_SSP[3][5]; // 3 ROWS (0, 1, 2) AND 5 COLUMNS (0, 1, 2, 3, 4)
    int int_size_SSP;
    int iArray_size_SSP;
    int iArray_num_elements_SSP, iArray_num_rows_SSP, iArray_num_columns_SSP;
    int i_SSP, j_SSP;
    //code
    printf("\n\n");
    int_size_SSP = sizeof(int);
    iArray_size_SSP = sizeof(iArray_SSP);
    printf("Size Of Two Dimensional ( 2D ) Integer Array Is = %d\n\n", iArray_size_SSP);
    iArray_num_rows_SSP = iArray_size_SSP / sizeof(iArray_SSP[0]);
    printf("Number of Rows In Two Dimensional ( 2D ) Integer Array Is = %d\n\n", iArray_num_rows_SSP);
    iArray_num_columns_SSP = sizeof(iArray_SSP[0]) / int_size_SSP;
    printf("Number of Columns In Two Dimensional ( 2D ) Integer Array Is = %d\n\n", iArray_num_columns_SSP);
    iArray_num_elements_SSP = iArray_num_rows_SSP * iArray_num_columns_SSP;
    printf("Number of Elements In Two Dimensional ( 2D ) Integer Array Is = %d\n\n", iArray_num_elements_SSP);
    printf("\n\n");
    printf("Elements In The 2D Array : \n\n");
    // ****** PIECE-MEAL ASSIGNMENT ******
    // ****** ROW 1 ******  
    iArray_SSP[0][0] = 21;
    iArray_SSP[0][1] = 42;
    iArray_SSP[0][2] = 63;
    iArray_SSP[0][3] = 84;
    iArray_SSP[0][4] = 105;
    // ****** ROW 2 ******
    iArray_SSP[1][0] = 22;
    iArray_SSP[1][1] = 44;
    iArray_SSP[1][2] = 66;
    iArray_SSP[1][3] = 88;
    iArray_SSP[1][4] = 110;
    // ****** ROW 3 ******
    iArray_SSP[2][0] = 23;
    iArray_SSP[2][1] = 46;
    iArray_SSP[2][2] = 69;
    iArray_SSP[2][3] = 92;
    iArray_SSP[2][4] = 115;
    // *** DISPLAY ***
    for (i_SSP = 0; i_SSP < iArray_num_rows_SSP; i_SSP++)
    {
        printf("****** ROW %d ******\n", (i_SSP + 1));
        for (j_SSP = 0; j_SSP < iArray_num_columns_SSP; j_SSP++)
        {
            printf("iArray[%d][%d] = %d\n", i_SSP, j_SSP, iArray_SSP[i_SSP][j_SSP]);
        }
        printf("\n\n");
    }
    return(0);
}
/*
Size Of Two Dimensional ( 2D ) Integer Array Is = 60

Number of Rows In Two Dimensional ( 2D ) Integer Array Is = 3

Number of Columns In Two Dimensional ( 2D ) Integer Array Is = 5

Number of Elements In Two Dimensional ( 2D ) Integer Array Is = 15



Elements In The 2D Array :

****** ROW 1 ******
iArray[0][0] = 21
iArray[0][1] = 42
iArray[0][2] = 63
iArray[0][3] = 84
iArray[0][4] = 105


****** ROW 2 ******
iArray[1][0] = 22
iArray[1][1] = 44
iArray[1][2] = 66
iArray[1][3] = 88
iArray[1][4] = 110


****** ROW 3 ******
iArray[2][0] = 23
iArray[2][1] = 46
iArray[2][2] = 69
iArray[2][3] = 92
iArray[2][4] = 115
*/