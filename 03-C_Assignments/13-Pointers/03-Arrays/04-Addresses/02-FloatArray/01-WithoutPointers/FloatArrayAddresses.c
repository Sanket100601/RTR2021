#include<stdio.h>

int  main(void)
{
    // variable declarations
    float fArray_SSP[10];
    int i_SSP;

    // code
    for(i_SSP=0;i_SSP<10;i_SSP++)
    {
        fArray_SSP[i_SSP]=(float)(i_SSP+1)*1.5f;
    }

    printf("\n\n");
    printf("Elements Of The Integers Array ; \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    {
        printf("fArray[%d] = %f\n", i_SSP, fArray_SSP[i_SSP]);
    }

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    {
        printf("fArray[%d] = %f \t \t Address = %p\n", i_SSP, fArray_SSP[i_SSP], &fArray_SSP[i_SSP]);
    }
    
    printf("\n\n");

    return 0;
}