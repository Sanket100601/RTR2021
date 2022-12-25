#include <stdio.h>
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
    union MyUnion u1_SSP, u2_SSP;
   
    //code
    // *** MyUnion u2 ***
    printf("\n\n");
    printf("Members Of Union u1 Are : \n\n");
    
    u1_SSP.i_SSP = 6;
    u1_SSP.f_SSP = 1.2f;
    u1_SSP.d_SSP = 8.333333;
    u1_SSP.c_SSP = 'S';

    printf("u1.i = %d\n\n", u1_SSP.i_SSP);
    printf("u1.f = %f\n\n", u1_SSP.f_SSP);
    printf("u1.d = %lf\n\n", u1_SSP.d_SSP);
    printf("u1.c = %c\n\n", u1_SSP.c_SSP);
 
    printf("Addresses Of Members Of Union u1 Are : \n\n");
    printf("u1.i = %p\n\n", &u1_SSP.i_SSP);
    printf("u1.f = %p\n\n", &u1_SSP.f_SSP);
    printf("u1.d = %p\n\n", &u1_SSP.d_SSP);
    printf("u1.c = %p\n\n", &u1_SSP.c_SSP);

    printf("MyUnion u1 = %p\n\n", &u1_SSP);

    // *** MyUnion u2 ***
    printf("\n\n");
    printf("Members Of Union u2 Are : \n\n");

    u2_SSP.i_SSP = 3;
    printf("u2.i = %d\n\n", u2_SSP.i_SSP);

    u2_SSP.f_SSP = 4.5f;
    printf("u2.f = %f\n\n", u2_SSP.f_SSP);

    u2_SSP.d_SSP = 5.12764;
    printf("u2.d = %lf\n\n", u2_SSP.d_SSP);

    u2_SSP.c_SSP = 'A';
    printf("u2.c = %c\n\n", u2_SSP.c_SSP);

    printf("Addresses Of Members Of Union u2 Are : \n\n");
    printf("u2.i = %p\n\n", &u2_SSP.i_SSP);
    printf("u2.f = %p\n\n", &u2_SSP.f_SSP);
    printf("u2.d = %p\n\n", &u2_SSP.d_SSP);
    printf("u2.c = %p\n\n", &u2_SSP.c_SSP);
   
    printf("MyUnion u2 = %p\n\n", &u2_SSP);
    
    return(0);
}