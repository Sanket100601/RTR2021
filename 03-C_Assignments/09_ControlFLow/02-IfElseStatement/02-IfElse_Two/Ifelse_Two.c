#include<stdio.h>
int main(void)
{
    //variable declaration
    int age_SSP;

    //code
    printf("\n\n");
    printf("Enter Age: ");
    scanf("%d",&age_SSP);
    printf("\n\n");
    if(age_SSP>=18)
    {
        printf("Entering the first if block...\n\n");
        printf("You are Eligible for Voting!!!\n\n");
    }

    else
    {
        printf("entering the first else block...\n\n");
        printf("You are NOT Eligible for Voting!!!\n\n");
    }
    printf("BYE!!!\n\n");
    return 0;
}
/*
Enter Age: 20


Entering the first if block...

You are Eligible for Voting!!!

BYE!!!
*/