#include<stdio.h>
int main(void)
{
    //variable declaration
    int i_SSP,j_SSP;

    //code
    printf("\n\n");

    printf("printing Digits 10 to 1 and 100 to 10: \n\n");

    for(i_SSP=10,j_SSP=100;i_SSP>=1,j_SSP>=10;i_SSP--,j_SSP-=10)
    {
        printf("\t %d \t %d \n",i_SSP,j_SSP);
    }
    printf("\n\n");

    return 0;
}
/*
printing Digits 10 to 1 and 100 to 10: 

         10      100
         9       90
         8       80
         7       70
         6       60
         5       50
         4       40
         3       30
         2       20
         1       10
*/