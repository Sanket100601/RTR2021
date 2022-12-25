#include<stdio.h>
int main(void)
{
    //variable declaration
    int age_SSP;

    //code
    printf("\n\n");
    printf("Enter Age:");
    scanf("%d",&age_SSP);
    if(age_SSP >= 18)
    {
        printf("you  are eligible for voting!!!\n\n");
    }
    printf("You are not eligible for Voting!!!\n\n");
    return 0;
}

/*
Enter Age:20
you  are eligible for voting!!!

You are not eligible for Voting!!!
*/