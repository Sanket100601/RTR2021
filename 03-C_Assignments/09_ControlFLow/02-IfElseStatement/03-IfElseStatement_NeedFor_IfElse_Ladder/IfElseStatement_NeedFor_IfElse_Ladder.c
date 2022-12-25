#include<stdio.h>
int main(void)
{
    //variable declaration 
    int num_SSP;

    //code
    printf("\n\n");
    printf("Enter the value for num_SSP: ");
    scanf("%d",&num_SSP);

    if(num_SSP<0)   //'if'-01
    {
        printf("Num = %d Is Less Than 0 (NEGATIVE)!!!",num_SSP);
    }
    else    //'else'-01
    {
        if((num_SSP>0)&&(num_SSP<=100)) //'if'-02
        {
            printf("Num= %d Is Between 0 and 100 !!!\n\n",num_SSP);
        }
        else    //else-02
        {
            if((num_SSP>100)&&(num_SSP<=200))   //if-03
            {
                printf("Num= %d Is Between 100 and 200 !!!\n\n",num_SSP);
            }
            else    //else-03
            {
                if((num_SSP>200)&&(num_SSP<=300))   //if-04
                {
                printf("Num= %d Is Between 200 and 300 !!!\n\n",num_SSP);
                }
                else    //else-04
                {
                    if ((num_SSP>300)&&(num_SSP<=400))  //if-05
                    {
                        printf("Num= %d Is Between 300 and 400 !!!\n\n",num_SSP);
                    }
                    else    //else-05
                    {
                        if ((num_SSP>400)&&(num_SSP<=500))  //if-06
                        {
                            printf("Num = %d Is Between 400 and 500 !!!\n\n",num_SSP);
                        }
                        else    //else-06
                        {
                            printf("Num= %d Is Greater than 500 !!!\n\n",num_SSP);
                        }
                        
                    }
                    
                }
            }
        }
    }

    return 0;
}
/*
nter the value for num_SSP: 20
Num= 20 Is Between 0 and 100 !!!
*/