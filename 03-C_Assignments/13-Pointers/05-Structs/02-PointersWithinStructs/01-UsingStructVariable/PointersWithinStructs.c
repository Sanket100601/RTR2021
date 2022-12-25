#include <stdio.h>

// DEFINING STRUCT
struct MyData
{
    int *ptr_i_SSP;
    int i_SSP;
    float *ptr_f_SSP;
    float f_SSP;
    double *ptr_d_SSP;
    double d_SSP;
};

int main(void)
{
    //variable declarations
    struct MyData data_SSP;
   
    //code
    data_SSP.i_SSP = 9;
    data_SSP.ptr_i_SSP = &data_SSP.i_SSP;
   
    data_SSP.f_SSP = 11.45f;
    data_SSP.ptr_f_SSP = &data_SSP.f_SSP;
   
    data_SSP.d_SSP = 30.121995;
    data_SSP.ptr_d_SSP = &data_SSP.d_SSP;
   
    printf("\n\n");
    printf("i = %d\n", *(data_SSP.ptr_i_SSP));
    printf("Adress Of 'i' = %p\n", data_SSP.ptr_i_SSP);
   
    printf("\n\n");
    printf("f = %f\n", *(data_SSP.ptr_f_SSP));
    printf("Adress Of 'f' = %p\n", data_SSP.ptr_f_SSP);
   
    printf("\n\n");
    printf("d = %lf\n", *(data_SSP.ptr_d_SSP));
    printf("Adress Of 'd' = %p\n", data_SSP.ptr_d_SSP);
   
    return(0);
}