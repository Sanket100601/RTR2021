#include <stdio.h>
int main(void)
{
    // variable declaration
    int iArray_One_SSP[5];
    int iArray_Two_SSP[5][3];
    int iArray_Three_SSP[100][100][5];
    int num_rows_2D_SSP;
    int num_columns_2D_SSP;
    int num_rows_3D_SSP;
    int num_columns_3D_SSP;
    int depth_3D_SSP;
    
    // code
    printf("\n\n");
    printf("Size of 1-D integer array iArray_One = %lu\n", sizeof(iArray_One_SSP));
    printf("Number of elements in 1-D integer array iArray_One = %lu\n", (sizeof(iArray_One_SSP) / sizeof(int)));
    
    printf("\n\n");
    printf("Size of 2-D integer array iArray_Two = %lu\n", sizeof(iArray_Two_SSP));
    printf("Number rows in 2-D integer array iArray_Two = %lu\n", (sizeof(iArray_Two_SSP) / sizeof(iArray_Two_SSP[0])));
    num_rows_2D_SSP = (sizeof(iArray_Two_SSP) / sizeof(iArray_Two_SSP[0]));
    
    printf("Number of elements (columns) in each row in 2-D integer array iArray_Two = %lu\n", (sizeof(iArray_Two_SSP[0]) / sizeof(iArray_Two_SSP[0][0])));
    num_columns_2D_SSP = (sizeof(iArray_Two_SSP[0]) / sizeof(iArray_Two_SSP[0][0]));
    
    printf("Number of elements in total in 2-D Array iArray_Two = %d\n", (num_rows_2D_SSP * num_columns_2D_SSP));
    printf("\n\n");
    
    printf("\n\n");
    printf("Size of 3-D integer array iArray_Three = %lu\n", sizeof(iArray_Three_SSP));
    
    printf("Number rows in 3-D integer array iArray_Three = %lu\n", (sizeof(iArray_Three_SSP) / sizeof(iArray_Three_SSP[0])));
    num_rows_3D_SSP = (sizeof(iArray_Three_SSP) / sizeof(iArray_Three_SSP[0]));
    printf("Number of elements (columns) in one row in 3-D integer array iArray_Three = %lu\n", (sizeof(iArray_Three_SSP[0]) / sizeof(iArray_Three_SSP[0][0])));
    
    num_columns_3D_SSP = (sizeof(iArray_Three_SSP[0]) / sizeof(iArray_Three_SSP[0][0]));
    printf("Number of elements (depth) in one column in one row in 3-D integer array iArray_Three = %lu\n", (sizeof(iArray_Three_SSP[0][0]) / sizeof(iArray_Three_SSP[0][0][0])));
    
    depth_3D_SSP = (sizeof(iArray_Three_SSP[0][0]) / sizeof(iArray_Three_SSP[0][0][0]));
    printf("Number of elements in total in 3-D Array iArray_Three = %d\n", (num_rows_3D_SSP * num_columns_3D_SSP * depth_3D_SSP));
    
    printf("\n\n");
    return(0);
}
/*
Size of 1-D integer array iArray_One = 20
Number of elements in 1-D integer array iArray_One = 5


Size of 2-D integer array iArray_Two = 60
Number rows in 2-D integer array iArray_Two = 5
Number of elements (columns) in each row in 2-D integer array iArray_Two = 3
Number of elements in total in 2-D Array iArray_Two = 15




Size of 3-D integer array iArray_Three = 200000
Number rows in 3-D integer array iArray_Three = 100
Number of elements (columns) in one row in 3-D integer array iArray_Three = 100
Number of elements (depth) in one column in one row in 3-D integer array iArray_Three = 5
Number of elements in total in 3-D Array iArray_Three = 50000
*/