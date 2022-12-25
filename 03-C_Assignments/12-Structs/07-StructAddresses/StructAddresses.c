#include<stdio.h>

struct MyData
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
    char c_SSP;
};

int main(void)
{
    // variable Declaration
    struct MyData data_SSP;

    // code
    // Assingments Data Values To The Data Members Of 'struct MyData'
    data_SSP.i_SSP = 30;
    data_SSP.f_SSP = 11.45f;
    data_SSP.d_SSP = 1.2995;
    data_SSP.c_SSP = 'A';

    //Displaying Values Of The Data Members Of 'struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n", data_SSP.i_SSP);
    printf("f = %f\n", data_SSP.f_SSP);
    printf("d = %lf\n", data_SSP.d_SSP);
    printf("c = %c\n\n", data_SSP.c_SSP);

    printf("\n\n");
    printf("ADDRESSES OF DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("'i' Occupies Addresses From %p\n", &data_SSP.i_SSP);
    printf("'f' Occupies Addresses From %p\n", &data_SSP.f_SSP);
    printf("'d' Occupies Addresses From %p\n", &data_SSP.d_SSP);
    printf("'c' Occupies Address %p\n\n", &data_SSP.c_SSP);

    printf("Starting Address Of 'struct MyData' variable 'data' = %p\n\n", &data_SSP);

    return(0);
}