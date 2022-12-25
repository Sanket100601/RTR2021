#include <stdio.h>
int main(void)
{
    //variable declaraions
    int iArray_SSP[5][3] = { {1, 2, 3}, {2, 4, 6}, {3, 6, 9}, {4, 8, 12}, {5, 10, 15} }; //IN-LINE INITIALIZATION
    int int_size_SSP;
    int iArray_size_SSP;
    int iArray_num_elements_SSP, iArray_num_rows_SSP, iArray_num_columns_SSP;
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
    // *** ARRAY INDICES BEGIN FROM 0, HENCE, 1ST ROW IS ACTUALLY 0TH ROW AND 1ST COLUMN IS ACTUALLY 0TH COLUMN ***
    // *** ROW 1 ***
    printf("****** ROW 1 ******\n");
    printf("iArray[0][0] = %d\n", iArray_SSP[0][0]); // *** COLUMN 1 *** (0th Element)=> 1
    printf("iArray[0][1] = %d\n", iArray_SSP[0][1]); // *** COLUMN 2 *** (1st Element)=> 2
    printf("iArray[0][2] = %d\n", iArray_SSP[0][2]); // *** COLUMN 3 *** (2nd Element)=> 3
    printf("\n\n");
    // *** ROW 2 ***
    printf("****** ROW 2 ******\n");
    printf("iArray[1][0] = %d\n", iArray_SSP[1][0]); // *** COLUMN 1 *** (0th Element)=> 2
    printf("iArray[1][1] = %d\n", iArray_SSP[1][1]); // *** COLUMN 2 *** (1st Element)=> 4
    printf("iArray[1][2] = %d\n", iArray_SSP[1][2]); // *** COLUMN 3 *** (2nd Element)=> 6
    printf("\n\n");
    // *** ROW 3 ***
    printf("****** ROW 3 ******\n");
    printf("iArray[2][0] = %d\n", iArray_SSP[2][0]); // *** COLUMN 1 *** (0th Element)=> 3
    printf("iArray[2][1] = %d\n", iArray_SSP[2][1]); // *** COLUMN 2 *** (1st Element)=> 6
    printf("iArray[2][2] = %d\n", iArray_SSP[2][2]); // *** COLUMN 3 *** (2nd Element)=> 9
    printf("\n\n");
    // *** ROW 4 ***
    printf("****** ROW 4 ******\n");
    printf("iArray[3][0] = %d\n", iArray_SSP[3][0]); // *** COLUMN 1 *** (0th Element)=> 4
    printf("iArray[3][1] = %d\n", iArray_SSP[3][1]); // *** COLUMN 2 *** (1st Element)=> 8
    printf("iArray[3][2] = %d\n", iArray_SSP[3][2]); // *** COLUMN 3 *** (2nd Element)=> 12
    printf("\n\n");
    // *** ROW 5 ***
    printf("****** ROW 5 ******\n");
    printf("iArray[4][0] = %d\n", iArray_SSP[4][0]); // *** COLUMN 1 *** (0th Element)=> 5
    printf("iArray[4][1] = %d\n", iArray_SSP[4][1]); // *** COLUMN 2 *** (1st Element)=> 10
    printf("iArray[4][2] = %d\n", iArray_SSP[4][2]); // *** COLUMN 3 *** (2nd Element)=> 15
    printf("\n\n");
    return(0);
}
/*
Size Of Two Dimensional ( 2D ) Integer Array Is = 60

Number of Rows In Two Dimensional ( 2D ) Integer Array Is = 5

Number of Columns In Two Dimensional ( 2D ) Integer Array Is = 3

Number of Elements In Two Dimensional ( 2D ) Integer Array Is = 15



Elements In The 2D Array : 

****** ROW 1 ******
iArray[0][0] = 1
iArray[0][1] = 2
iArray[0][2] = 3


****** ROW 2 ******
iArray[1][0] = 2
iArray[1][1] = 4
iArray[1][2] = 6


****** ROW 3 ******
iArray[2][0] = 3
iArray[2][1] = 6
iArray[2][2] = 9


****** ROW 4 ******
iArray[3][0] = 4
iArray[3][1] = 8
iArray[3][2] = 12


****** ROW 5 ******
iArray[4][0] = 5
iArray[4][1] = 10
iArray[4][2] = 15
*/
