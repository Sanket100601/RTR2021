#include<stdio.h>
int main(void)
{
    //variable declarations
    int i_SSP;

    //code
    printf("\n\n");

    printf("Printing digits 10 to 1: \n\n");

    i_SSP=10;
    do
    {
        printf("\t %d \n",i_SSP);
        i_SSP--;
    } while (i_SSP>=1);
    
    printf("\n\n");

    return 0;
    
}
