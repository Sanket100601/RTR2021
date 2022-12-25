#include<stdio.h>
int main(void)
{
    //variable declaration
    int  age_SSP;

    //code
    printf("\n\n");
    printf("Enter Age: ");
    scanf("%d",&age_SSP);
    if(age_SSP>=18)
    {
        printf("You are Eligible for Voting!!!\n\n");
    }
    return 0;
}
/*
Enter Age: 23
You are Eligible for Voting!!!
*/
