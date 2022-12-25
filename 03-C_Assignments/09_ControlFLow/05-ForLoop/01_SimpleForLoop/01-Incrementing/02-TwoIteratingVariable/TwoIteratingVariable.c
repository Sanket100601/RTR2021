#include<stdio.h>
int main(void)
{
    //variable declaration
    int i_SSP,j_SSP;

    //code
    printf("\n\n");

    printf("printing Digits 1 to 10 and 10 to 100: \n\n");

    for(i_SSP=1,j_SSP=10;i_SSP<=10,j_SSP<=100;i_SSP++,j_SSP+=10)
    {
        printf("\t %d \t %d \n",i_SSP,j_SSP);
    }
    printf("\n\n");

    return 0;
}
/*
printing Digits 1 to 10 and 10 to 100: 

         1       10
         2       20
         3       30
         4       40
         5       50
         6       60
         7       70
         8       80
         9       90
         10      100
*/