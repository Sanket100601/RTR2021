#include <stdio.h>
int main(void)
{
    //variable declaraions
    //IN-LINE INITIALIZATION
    int iArray_SSP[5][3][2] = { { { 9, 18 }, { 27, 36 }, { 45, 54 } },
                            { { 8, 16 }, { 24, 32 }, { 40, 48 } },
                            { { 7, 14 }, { 21, 28 }, { 35, 42 } },
                            { { 6, 12 }, { 18, 24 }, { 30, 36 } },
                            { { 5, 10 }, { 15, 20 }, { 25, 30 } } };
    int int_size_SSP;
    int iArray_size_SSP;
    int iArray_num_elements_SSP, iArray_width_SSP, iArray_height_SSP, iArray_depth_SSP;
    int i_SSP, j_SSP, k_SSP;
    
    //code
    printf("\n\n");
    
    int_size_SSP = sizeof(int);
    
    iArray_size_SSP = sizeof(iArray_SSP);
    printf("Size Of Three Dimensional ( 3D ) Integer Array Is = %d\n\n", iArray_size_SSP);
    
    iArray_width_SSP = iArray_size_SSP / sizeof(iArray_SSP[0]);
    printf("Number of Rows (Width) In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", iArray_width_SSP);
    
    iArray_height_SSP = sizeof(iArray_SSP[0]) / sizeof(iArray_SSP[0][0]);
    printf("Number of Columns (Height) In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", iArray_height_SSP);
    
    iArray_depth_SSP = sizeof(iArray_SSP[0][0]) / int_size_SSP;
    printf("Depth In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", iArray_depth_SSP);
    
    iArray_num_elements_SSP = iArray_width_SSP * iArray_height_SSP * iArray_depth_SSP;
    printf("Number of Elements In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", iArray_num_elements_SSP);
    
    printf("\n\n");
    printf("Elements In Integer 3D Array : \n\n");
    for (i_SSP = 0; i_SSP < iArray_width_SSP; i_SSP++)
    {
        printf("****** ROW %d ******\n", (i_SSP + 1));
        for (j_SSP = 0; j_SSP < iArray_height_SSP; j_SSP++)
        {
            printf("****** COLUMN %d ******\n", (j_SSP + 1));
            for (k_SSP = 0; k_SSP < iArray_depth_SSP; k_SSP++)
            {
                printf("iArray[%d][%d][%d] = %d\n", i_SSP, j_SSP, k_SSP, iArray_SSP[i_SSP][j_SSP][k_SSP]);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    return(0);
}
/*
Size Of Three Dimensional ( 3D ) Integer Array Is = 120

Number of Rows (Width) In Three Dimensional ( 3D ) Integer Array Is = 5

Number of Columns (Height) In Three Dimensional ( 3D ) Integer Array Is = 3

Depth In Three Dimensional ( 3D ) Integer Array Is = 2

Number of Elements In Three Dimensional ( 3D ) Integer Array Is = 30



Elements In Integer 3D Array :

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
*/
