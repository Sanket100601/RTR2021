#include<stdio.h>
int main(void)
{
    //variable declaration 
    int num_month_SSP;

    //code
    printf("\n\n");

    printf("Enter Number of the Month (1 to 12) : ");
    scanf("%d",&num_month_SSP);

    printf("\n\n");

    //if else if ladder begins 
    if (num_month_SSP==1)
    {
        /* code */
        printf("Month Number %d is January",num_month_SSP);
    }
    else if (num_month_SSP==2)
    {
        /* code */
        printf("Month Number %d is February",num_month_SSP);
    }
    else if (num_month_SSP==3)
    {
        /* code */
        printf("Month Number %d is March",num_month_SSP);
    }
    else if (num_month_SSP==4)
    {
        /* code */
        printf("Month Number %d is April",num_month_SSP);
    }
    else if (num_month_SSP==5)
    {
        /* code */
        printf("Month Number %d is May",num_month_SSP);
    }
    else if (num_month_SSP==6)
    {
        /* code */
        printf("Month Number %d is June",num_month_SSP);
    }
    else if (num_month_SSP==7)
    {
        /* code */
        printf("Month Number %d is July",num_month_SSP);
    }
    else if (num_month_SSP==8)
    {
        /* code */
        printf("Month Number %d is August",num_month_SSP);
    }
    else if (num_month_SSP==9)
    {
        /* code */
        printf("Month Number %d is September",num_month_SSP);
    }
    else if (num_month_SSP==10)
    {
        /* code */
        printf("Month Number %d is October",num_month_SSP);
    }
    else if (num_month_SSP==11)
    {
        /* code */
        printf("Month Number %d is November",num_month_SSP);
    }
    else if (num_month_SSP==12)
    {
        /* code */
        printf("Month Number %d is December",num_month_SSP);
    }
    else
    {
        printf("Invalid Month Number %d is Entered !!! Please Try Again...\n\n",num_month_SSP);
    }
    
    printf("\nIf-Else-If Ladder Complete !!!\n");
    
    return 0;
}