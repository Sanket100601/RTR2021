#include<stdio.h>
int main(void)
{
    //variable declaration
    int num_SSP;

    //code
    printf("\n\n");

    printf("Enter the Value for 'num_SSP': ");
    scanf("%d",&num_SSP);

    //if-else-if ladder begins from here 
    if(num_SSP<0)
    {
        printf("Num = %d Is Less Than 0 (NEGATIVE) !!!\n\n",num_SSP);
    }

    else if((num_SSP>0)&&(num_SSP<=100))
        printf("Num = %d Is Between 0 And 100 !!!\n\n",num_SSP);
    
    else if((num_SSP>100)&&(num_SSP<=200))
        printf("Num = %d Is Between 100 And 200 !!!\n\n",num_SSP);
        
    else if((num_SSP>200)&&(num_SSP<=300))
        printf("Num = %d Is Between 200 And 300 !!!\n\n",num_SSP);

    else if((num_SSP>300)&&(num_SSP<=400))
        printf("Num = %d Is Between 300 And 400 !!!\n\n",num_SSP);

    else if((num_SSP>400)&&(num_SSP<=500))
        printf("Num = %d Is Between 400 And 500 !!!\n\n",num_SSP);

    else if((num_SSP>500))
        printf("Num = %d Is greater than 500 !!!\n\n",num_SSP);

    return 0;
}
/*
Enter the Value for 'num_SSP': 20
Num = 20 Is Between 0 And 100 !!!
*/