#include<stdio.h>

int main(void)
{
    // variable declaration
    int num_SSP;
    int *ptr_SSP=NULL;
    int **pptr_SSP=NULL;      //Declaration method 1:-**ptr is a variable of type int

    // code 
    num_SSP=10;

    printf("\n\n");
    
    printf("***** BEFORE ptr = &num *****\n\n");

    printf("value of num = %d\n\n",num_SSP);
    printf("Address of num = %p\n\n",&num_SSP);
    printf("Value at address of num = %d",*(&num_SSP));

    //Assigning address of variable 'num' to pointer variable 'ptr'
    //'ptr' now contains address of 'num'...hence, 'ptr' is SAME as '&num'
    ptr_SSP = &num_SSP;

    printf("\n\n");

    printf("***** AFTER ptr = &num *****\n\n");
    printf("value of num = %d\n\n",num_SSP);
    printf("Address of num = %p\n\n",&num_SSP);
    printf("Value at address of num = %d",*ptr_SSP);

    // Assigning address of variable 'ptr' to pointer-to-pointer variable 'pptr'
    // 'pptr' now contains the address of 'ptr' which in turn contains the address of 'num'
    // Hence, 'pptr' is SAME as '&ptr'
    // 'ptr' is SAME as '&num'
    // Hence, pptr = &ptr = &(&num)
    // If ptr = &num and *ptr = *(&num) = value at address of 'num'
    // Then, pptr = &ptr and *pptr = *(&ptr) = ptr = value at address of 'ptr' i.e: 'ptr' i.e : address of 'num'
    // Then, **pptr = **(&ptr) = *(*(&ptr)) = *ptr = *(&num) = num = 10
    // Hence, num = *(&num) = *ptr = *(*pptr) = **pptr 
    pptr_SSP = &ptr_SSP;

    printf("\n\n");

    printf(" ****** AFTER pptr = &ptr ******\n\n");
    printf("Value Of 'num'                           = %d\n\n", num_SSP);
    printf("Address Of 'num' (ptr)                   = %p\n\n", ptr_SSP);
    printf("Address Of 'ptr'  (pptr)                 = %p\n\n", pptr_SSP);
    printf("Value At Address Of 'ptr' (*pptr)        = %p\n\n", *pptr_SSP);
    printf("Value At Address Of 'num' (*ptr) (*pptr) = %d\n\n", **pptr_SSP);
    return(0);
}
