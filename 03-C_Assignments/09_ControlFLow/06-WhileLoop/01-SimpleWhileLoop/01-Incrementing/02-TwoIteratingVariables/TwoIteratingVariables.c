#include<stdio.h>
int main(void)
{
    //variable declarationns
    int i_SSP,j_SSP;

    //code
    printf("\n\n");

    printf("Printing digits 1 to 10 and 10 to 100: \n\n");

    i_SSP=1;
    j_SSP=10;
    while(i_SSP<=10,j_SSP<=100)
    {
        printf("\t %d \t %d \n",i_SSP,j_SSP);
        i_SSP++;
        j_SSP=j_SSP+10;
    }

    printf("\n\n");

    return 0;
}
/*
Printing digits 1 to 10 and 10 to 100: 

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
