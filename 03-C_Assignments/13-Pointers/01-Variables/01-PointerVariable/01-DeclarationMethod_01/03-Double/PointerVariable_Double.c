#include<stdio.h>

int main(void)
{
    // variable declaration
    double num_SSP;
    double *ptr_SSP=NULL;   //*ptr_SSP is variable of type int

    // code
    num_SSP=2.34343434f;

    printf("\n\n");

    printf(" ***** Before ptr = &num *****\n\n");
    printf("Value Of 'num'              =%lf\n\n",num_SSP);
    printf("Address Of 'num'            =%p\n\n",&num_SSP);
    printf("Value At Address Of 'num'   =%lf\n\n",*(&num_SSP));

    //Assinging adress of variable 'num' to pointer variable 'ptr'
    //'ptr' now contains adress of 'num'...Hence, 'ptr' is SAME as '&num'
    ptr_SSP = &num_SSP;

    printf("\n\n");

    printf(" ***** AFTER ptr = &num *****\n\n");
    printf("Value Of 'num'              =%lf\n\n",num_SSP);
    printf("Address Of 'num'            =%p\n\n",ptr_SSP);
    printf("Value At Adsress Of 'num'   =%lf\n\n",*ptr_SSP);

    return 0;
}