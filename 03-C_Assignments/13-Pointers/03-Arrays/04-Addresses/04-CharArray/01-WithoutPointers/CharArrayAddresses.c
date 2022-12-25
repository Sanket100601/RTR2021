#include<stdio.h>

int  main(void)
{
    // variable declarations
    char cArray_SSP[10];
    int i_SSP;

    // code
    for(i_SSP=0;i_SSP<10;i_SSP++)
    {
        cArray_SSP[i_SSP]=(char)(i_SSP+65);
    }

    printf("\n\n");
    printf("Elements Of The Integers Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    {
        printf("cArray[%d] = %c\n", i_SSP, cArray_SSP[i_SSP]);
    }

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    {
        printf("cArray[%d] = %c \t \t Address = %p\n", i_SSP, cArray_SSP[i_SSP], &cArray_SSP[i_SSP]);
    }
    
    printf("\n\n");

    return 0;
}