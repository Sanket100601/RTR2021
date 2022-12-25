#include <stdio.h>
struct MyStruct
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
    char c_SSP;
};

union MyUnion
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
    char c_SSP;
};

int main(void)
{
    //variable declarations
    struct MyStruct s_SSP;
    union MyUnion u_SSP;

    //code
    printf("\n\n");
    printf("Size Of MyStruct = %lu\n", sizeof(s_SSP));
    printf("\n\n");
    printf("Size Of MyUnion = %lu\n", sizeof(u_SSP));

    return(0);
}