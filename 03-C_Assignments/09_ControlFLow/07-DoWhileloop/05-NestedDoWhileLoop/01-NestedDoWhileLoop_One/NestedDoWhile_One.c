#include<stdio.h>
int main(void)
{
    //variable declaration
    int i_SSP,j_SSP;

    //code
    printf("\n\n");
    i_SSP=1;
    do
    {
        /* code */
        printf("i = %d\n",i_SSP);
        printf("-------------------\n\n");

        j_SSP=1;
        do
        {
            /* code */
            printf("\tj_SSP=%d\n",j_SSP);
            j_SSP++;
        } while (j_SSP<=5);
        i_SSP++;
        printf("\n\n");
    }while(i_SSP<=10);
    return 0;
}
