#include<stdio.h>

int main(void)
{
    // variable declaration
    int num_SSP=5;
    const int* const ptr_SSP=&num_SSP;    // read this line from right to left => ptr is constant pointer to integer constant

    // code 
    printf("\n");
    printf("Current Value Of 'num' = %d\n",num_SSP);
    printf("Current ptr (Adddress of num) = %p\n",ptr_SSP);

    // The Following line does not give Error...as we are modifying the value of a variable individually 
    num_SSP++;
    printf("\n\n");
    printf("After 'num++', value of of 'num' = %d\n",num_SSP);
    return 0;

    // ptr++
    // *(ptr)++
    // both are not valid 
}