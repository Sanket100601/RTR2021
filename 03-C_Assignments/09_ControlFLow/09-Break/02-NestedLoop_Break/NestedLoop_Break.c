#include<stdio.h>
#include<stdio.h>
int main(void)
{
    //variable declarations
    int i_SSP,j_SSP;
    
    //code
    printf("\n\n");

    for(i_SSP=1;i_SSP<=20;i_SSP++)
    {
        for(j_SSP=1;j_SSP<=20;j_SSP++)
        {
            if(j_SSP>i_SSP)
            {
                break;
            }
            else
            {
                printf("* ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
    return 0;
}
/*
* 
* *
* * *
* * * *
* * * * *
* * * * * *
* * * * * * *
* * * * * * * *
* * * * * * * * *
* * * * * * * * * *
* * * * * * * * * * *
* * * * * * * * * * * *
* * * * * * * * * * * * *
* * * * * * * * * * * * * *
* * * * * * * * * * * * * * *
* * * * * * * * * * * * * * * *
* * * * * * * * * * * * * * * * *
* * * * * * * * * * * * * * * * * *
* * * * * * * * * * * * * * * * * * *
* * * * * * * * * * * * * * * * * * * *
*/