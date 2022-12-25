#include<stdio.h>

int main(void)
{
    // variable declaration
    int num_SSP;
    const int* ptr_SSP=NULL;    // read this line from right tto left => ptr is pointer to integer constant 

    // code 
    ptr_SSP = &num_SSP;
    printf("\n");
    printf("Current Value Of 'num' = %d\n",num_SSP);
    printf("Current ptr (Adddress of num) = %p\n",ptr_SSP);

    // The Following line does not give Error...as we are modifying the value of a variable individually 
    num_SSP++;
    printf("\n\n");
    printf("After 'num++', vcalue of of 'num' = %d\n",num_SSP);

    ptr_SSP++;

    printf("\n\n");
    printf("After ptr++ the value of ptr = %d\n",ptr_SSP);
    printf("Value at  this  new  ptr = %d\n",*ptr_SSP);
    printf("\n");
    return 0;
}
