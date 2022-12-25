#include<stdio.h>

int  main(void)
{
    // variable declarations
    int iArray_SSP[10];
    int i_SSP;

    // code
    for(i_SSP=0;i_SSP<10;i_SSP++)
    {
        iArray_SSP[i_SSP]=(i_SSP+1)*3;
    }

    printf("\n\n");
    printf("Elements Of The Integers Array ; \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    {
        printf("iArray[%d] = %d\n", i_SSP, iArray_SSP[i_SSP]);
    }

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    {
        printf("iArray[%d] = %d \t \t Address = %p\n", i_SSP, iArray_SSP[i_SSP], &iArray_SSP[i_SSP]);
    }
    
    printf("\n\n");

    return 0;
}