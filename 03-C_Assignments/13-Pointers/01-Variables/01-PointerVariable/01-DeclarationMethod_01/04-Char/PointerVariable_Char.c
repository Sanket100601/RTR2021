#include<stdio.h>

int main(void)
{
    // variable declaration
    char ch_SSP;
    double *ptr_SSP=NULL;   //*ptr_SSP is variable of type int

    // code
    ch_SSP='A';

    printf("\n\n");

    printf(" ***** Before ptr = &num *****\n\n");
    printf("Value Of 'num'              =%c\n\n",ch_SSP);
    printf("Address Of 'num'            =%p\n\n",&ch_SSP);
    printf("Value At Address Of 'num'   =%c\n\n",*(&ch_SSP));

    //Assinging adress of variable 'num' to pointer variable 'ptr'
    //'ptr' now contains adress of 'num'...Hence, 'ptr' is SAME as '&num'
    ptr_SSP = &ch_SSP;

    printf("\n\n");

    printf(" ***** AFTER ptr = &num *****\n\n");
    printf("Value Of 'num'              =%c\n\n",ch_SSP);
    printf("Address Of 'num'            =%p\n\n",ptr_SSP);
    printf("Value At Adsress Of 'num'   =%c\n\n",*ptr_SSP);

    return 0;
}