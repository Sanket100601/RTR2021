#include<stdio.h>

int main(void)
{
    // function declaration
    void SwapNumbers(int,int);

    // variable declaration
    int a_SSP;
    int b_SSP;

    // code
    printf("\n\n");
    printf("Enter the Value For 'A' : ");
    scanf("%d",&a_SSP);

    printf("\n\n");
    printf("Enter the Value For 'B' : ");
    scanf("%d",&b_SSP);

    printf("\n\n");
    printf("****** BEFORE SWAPPING ******\n\n");
    printf("Value Of 'A' = %d\n\n", a_SSP);
    printf("Value Of 'B' = %d\n\n", b_SSP);

    SwapNumbers(a_SSP, b_SSP); // ****** ARGUMENTS PASSED 'BY VALUE' ... ******

    printf("\n\n");
    printf("****** AFTER SWAPPING ******\n\n");
    printf("Value Of 'A' = %d\n\n", a_SSP);
    printf("Value Of 'B' = %d\n\n", b_SSP);

    return 0;
}

void SwapNumbers(int x,int y)   //Value Of 'a' is copied into 'x' and value of 'b'is copied into 'y' ... swapping takes place between 'x' and 'y', not between 'a' and 'b' ...
{
    // varibale declaration
    int temp_SSP;

    // code 
    printf("\n\n");
    printf("***** BEFORE SWAPPING *****\n\n");
    printf("Value Of 'X' = %d\n\n", x);
    printf("Value Of 'Y' = %d\n\n", y);

    temp_SSP=x;
    x=y;
    y=temp_SSP;

    printf("\n\n");
    printf("****** AFTER SWAPPING ******\n\n");
    printf("Value Of 'X' = %d\n\n", x);
    printf("Value Of 'Y' = %d\n\n", y);

}
