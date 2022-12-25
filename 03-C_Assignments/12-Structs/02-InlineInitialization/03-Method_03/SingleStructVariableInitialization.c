#include <stdio.h>
int main(void)
{
//DEFINING STRUCT
struct MyData
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
    char c_SSP;
} data_SSP = { 5, 9.1f, 3.78623, 'N' };
    
    //Displaying Values Of The Data Members Of 'struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n", data_SSP.i_SSP);
    printf("f = %f\n", data_SSP.f_SSP);
    printf("d = %lf\n", data_SSP.d_SSP);
    printf("c = %c\n\n", data_SSP.c_SSP);
    
    return(0);
}