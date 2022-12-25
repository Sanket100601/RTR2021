#include<stdio.h>

int main(void)
{
    // variable declaration
    int num_SSP=5;
    int* const ptr_SSP=&num_SSP;    // read this line from right to left => ptr is constant pointer to integer 

    // code 
    printf("\n");
    printf("Current Value Of 'num' = %d\n",num_SSP);
    printf("Current ptr (Adddress of num) = %p\n",ptr_SSP);

    // The Following line does not give Error...as we are modifying the value of a variable individually 
    num_SSP++;
    printf("\n\n");
    printf("After 'num++', value of of 'num' = %d\n",num_SSP);

    (*ptr_SSP)++;

    printf("\n\n");
    printf("After (*ptr)++ the value of ptr = %p\n",ptr_SSP);
    printf("Value at  this  new  ptr = %d\n",*ptr_SSP);
    printf("\n");
    return 0;
}