#include<stdio.h>
int main(void)
{
    //variable declaration
    int i_SSP,j_SSP,k_SSP;

    //code
    printf("\n\n");

    i_SSP=1;
    do
    {
        /* code */
        printf("i=%d\n",i_SSP);
        printf("-----------\n\n");

        j_SSP=1;
        do
        {
            /* code */
            printf("\tj_SSP=%d\n",j_SSP);
            printf("\t---------\n\n");

            k_SSP=1;
            do
            {
                /* code */
                printf("\t\tk_SSP=%d\n",k_SSP);
                k_SSP++;
            } while (k_SSP<=3);
            printf("\n\n");
            j_SSP++;
            
        } while (j_SSP<=5);
            printf("\n\n");
            i_SSP++;
        
    } while (i_SSP<=10);
    return 0;
}