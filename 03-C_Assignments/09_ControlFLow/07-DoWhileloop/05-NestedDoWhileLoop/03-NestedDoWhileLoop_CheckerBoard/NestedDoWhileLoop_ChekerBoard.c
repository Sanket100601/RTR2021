#include<stdio.h>
int main(void)
{
    int i_SSP,j_SSP,c_SSP;

    //code
    printf("\n\n");

    i_SSP=0;
    do
    {
        j_SSP=0;
        do
        {
            c_SSP=((i_SSP & 0x8)==0) ^ ((j_SSP & 0x8)==0);
            if (c_SSP==0)
            {
                printf("  ");
            }
            if (c_SSP==1)
            {
                printf("* ");
            }
            j_SSP++;
        } while (j_SSP<64);
        printf("\n\n");
        i_SSP++;    
    } while (i_SSP<64);
    return 0;
}
    