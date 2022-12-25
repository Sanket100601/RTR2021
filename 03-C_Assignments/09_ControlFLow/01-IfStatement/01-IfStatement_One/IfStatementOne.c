#include <stdio.h>
int main(void)
{
    //variable declaration
    int a_SSP,b_SSP,p_SSP;

    //code 
    a_SSP=9;
    b_SSP=30;
    p_SSP=30;

    printf("\n\n");

    if(a_SSP<b_SSP)
    {
        printf("A_SSP is less than B_SSP!!!\n\n");
    }

    if(b_SSP!=p_SSP)
    {
        printf("B_SSP is not equal to P_SSP!!!\n\n");
    }

    printf("Both Comparisons Have Been Done!!!\n\n");
}
/*
A_SSP is less than B_SSP!!!

Both Comparisons Have Been Done!!!
*/