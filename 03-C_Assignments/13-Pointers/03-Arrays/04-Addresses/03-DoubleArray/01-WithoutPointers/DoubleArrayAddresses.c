#include<stdio.h>

int  main(void)
{
    // variable declarations
    double dArray_SSP[10];
    int i_SSP;

    // code
    for(i_SSP=0;i_SSP<10;i_SSP++)
    {
        dArray_SSP[i_SSP]=(float)(i_SSP+1)*1.3333333f;
    }

    printf("\n\n");
    printf("Elements Of The Integers Array ; \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    {
        printf("fArray[%d] = %lf\n", i_SSP, dArray_SSP[i_SSP]);
    }

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    {
        printf("dArray[%d] = %lf \t \t Address = %p\n", i_SSP, dArray_SSP[i_SSP], &dArray_SSP[i_SSP]);
    }
    
    printf("\n\n");

    return 0;
}