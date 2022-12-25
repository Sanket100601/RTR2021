#include<stdio.h>
int main(void)
{
    //variable declaration
    int a_SSP,b_SSP,p_SSP;

    //code
    a_SSP=9;
    b_SSP=30;
    p_SSP=30;

    //First if-else Pair
    printf("\n\n");
    if(a_SSP<b_SSP)
    {
        printf("Entering the first if block...\n\n");
        printf("A_SSP is less than B_SSP\n\n");
    }
    else
    {
        printf("Entering First else block...\n\n");
        printf("A_SSP is NOT less than B_SSP!!!\n\n");
    }
    printf("First if-else Pair Done!!!\n\n");

    //Second if-else Pair 
    printf("\n\n");
    if(b_SSP!=p_SSP)
    {
        printf("Entering the Second i Block...\n\n");
        printf("B_SSP is not eqaul to P_SSP!!!\n\n");
    }
    printf("Second if-else pair Done!!!\n\n");
    
    return 0;
}
/*
Entering the first if block...

A_SSP is less than B_SSP

First if-else Pair Done!!!



Second if-else pair Done!!!
*/