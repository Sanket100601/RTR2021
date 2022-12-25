#include<stdio.h>
int main(void)
{
    //variable declaration
    int a_SSP;

    //code
    printf("\n\n");

    a_SSP=5;
    if(a_SSP)   //Non zero positive value
    {
        printf("if block 1 : 'A' Exists and has value = %d!!!\n\n",a_SSP);
    }

    a_SSP=-5;
    if(a_SSP)   //Non zero negative value
    {
        printf("if block 2 : 'A' Exists and has value = %d!!!\n\n",a_SSP);
    }

    a_SSP=0;
    if(a_SSP)   //zero value
    {
        printf("if block 3 : 'A' Exists and has value = %d!!!\n\n",a_SSP);
    }

    printf("All three Statementds are done!!!\n\n");
    
    return 0;
}
/*
if block 1 : 'A' Exists and has value = 5!!!

if block 2 : 'A' Exists and has value = -5!!!

All three Statementds are done!!!
*/