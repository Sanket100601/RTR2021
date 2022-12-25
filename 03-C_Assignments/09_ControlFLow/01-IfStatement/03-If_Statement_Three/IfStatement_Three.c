#include<stdio.h>
int main(void)
{
    //variable declaration
    int num_SSP;

    //code
    printf("\n\n");

    printf("Enter the value for num_SSP: \n");
    scanf("%d",&num_SSP);

    if (num_SSP<0)
    {
        printf("Num = %d Is Less Than 0 (Negative).\n\n",num_SSP);
    }

    if ((num_SSP>0) && (num_SSP<=100))
    {
        printf("Num = %d Is Between 0 and 100.\n\n",num_SSP);
    }
    
    if((num_SSP>100)&&(num_SSP<=200))
    {
        printf("Num=%d is between 100 and 200.\n\n",num_SSP);
    }
    
    if((num_SSP>200)&&(num_SSP<=300))
    {
        printf("Num=%d is between 200 and 300.\n\n",num_SSP);
    }

    if((num_SSP>300)&&(num_SSP<=400))
    {
        printf("Num=%d is between 300 and 400.\n\n",num_SSP);
    }

    if((num_SSP>400)&&(num_SSP<=500))
    {
        printf("Num=%d is between 400 and 500.\n\n",num_SSP);
    }

    if(num_SSP>500)
    {
        printf("Num=%d is Greater than 500\n\n",num_SSP);
    }

    return 0;
}
/*
Enter the value for num_SSP: 
20
Num = 20 Is Between 0 and 100.
*/