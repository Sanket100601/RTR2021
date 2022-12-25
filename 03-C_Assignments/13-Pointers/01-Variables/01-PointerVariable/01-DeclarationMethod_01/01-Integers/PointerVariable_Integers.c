#include<stdio.h>

int main(void)
{
    // variable declaration
    int num_SSP;
    int *ptr_SSP=NULL;   //*ptr_SSP is variable of type int

    // code
    num_SSP=10;

    printf("\n\n");

    printf(" ***** Before ptr = &num *****\n\n");
    printf("Value Of 'num'              =%d\n\n",num_SSP);
    printf("Address Of 'num'            =%d\n\n",&num_SSP);
    printf("Value At Address Of 'num'   =%d\n\n",*(&num_SSP));

    //Assinging adress of variable 'num' to pointer variable 'ptr'
    //'ptr' now contains adress of 'num'...Hence, 'ptr' is SAME as '&num'
    ptr_SSP = &num_SSP;

    printf("\n\n");

    printf(" ***** AFTER ptr = &num *****\n\n");
    printf("Value Of 'num'              =%d\n\n",num_SSP);
    printf("Address Of 'num'            =%d\n\n",ptr_SSP);
    printf("Value At Adsress Of 'num'   =%d\n\n",*ptr_SSP);

    return 0;
}