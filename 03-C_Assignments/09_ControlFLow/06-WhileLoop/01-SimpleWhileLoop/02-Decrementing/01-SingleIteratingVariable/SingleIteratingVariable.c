#include<stdio.h>
int main(void)
{
    //variable declarations
    int i_SSP;

    //code
    printf("\n\n");

    printf("Printing digits 10 to 1: \n\n");

    i_SSP=10;
    while (i_SSP>=1)
    {
        /* code */
        printf("\t %d \n",i_SSP);
        i_SSP--;
    }
    printf("\n\n");

    return 0;
    
}
/*
Printing digits 10 to 1: 

         10
         9
         8
         7
         6
         5
         4
         3
         2
         1
*/