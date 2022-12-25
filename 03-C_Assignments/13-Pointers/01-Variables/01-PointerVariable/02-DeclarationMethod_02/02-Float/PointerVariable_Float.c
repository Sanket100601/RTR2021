#include<stdio.h>

int main(void)
{
    // variable declaration
    float num_SSP;
    float* ptr_SSP=NULL;   //*ptr_SSP is variable of type int

    // code
    num_SSP=6.9f;

    printf("\n\n");

    printf(" ***** Before ptr = &num *****\n\n");
    printf("Value Of 'num'              =%f\n\n",num_SSP);
    printf("Address Of 'num'            =%p\n\n",&num_SSP);
    printf("Value At Address Of 'num'   =%f\n\n",*(&num_SSP));

    //Assinging adress of variable 'num' to pointer variable 'ptr'
    //'ptr' now contains adress of 'num'...Hence, 'ptr' is SAME as '&num'
    ptr_SSP = &num_SSP;

    printf("\n\n");

    printf(" ***** AFTER ptr = &num *****\n\n");
    printf("Value Of 'num'              =%f\n\n",num_SSP);
    printf("Address Of 'num'            =%p\n\n",ptr_SSP);
    printf("Value At Adsress Of 'num'   =%f\n\n",*ptr_SSP);

    return 0;
}