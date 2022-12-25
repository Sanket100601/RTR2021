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
    do
    {
        printf("\t %d \t %d \n",i_SSP,j_SSP);
        i_SSP++;
        j_SSP=j_SSP+10;
    } while (i_SSP<=10,j_SSP<=100);

     printf("\n\n");

    return 0;
}
    