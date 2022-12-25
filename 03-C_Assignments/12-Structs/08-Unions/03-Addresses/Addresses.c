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
    printf("Members Of Struct Are : \n\n");
    s_SSP.i_SSP = 9;
    s_SSP.f_SSP = 8.7f;
    s_SSP.d_SSP = 1.233422;
    s_SSP.c_SSP = 'P';
  
    printf("s.i = %d\n\n", s_SSP.i_SSP);
    printf("s.f = %f\n\n", s_SSP.f_SSP);
    printf("s.d = %lf\n\n", s_SSP.d_SSP);
    printf("s.c = %c\n\n", s_SSP.c_SSP);

    printf("Addresses Of Members Of Struct Are : \n\n");
    printf("s.i = %p\n\n", &s_SSP.i_SSP);
    printf("s.f = %p\n\n", &s_SSP.f_SSP);
    printf("s.d = %p\n\n", &s_SSP.d_SSP);
    printf("s.c = %p\n\n", &s_SSP.c_SSP);
    printf("MyStruct s = %p\n\n", &s_SSP);
    printf("\n\n");
    
    printf("Members Of Union Are : \n\n");

    u_SSP.i_SSP = 3;
    printf("u.i = %d\n\n", u_SSP.i_SSP);
    
    u_SSP.f_SSP = 4.5f;
    printf("u.f = %f\n\n", u_SSP.f_SSP);
    
    u_SSP.d_SSP = 5.12764;
    printf("u.d = %lf\n\n", u_SSP.d_SSP);

    u_SSP.c_SSP = 'A';
    printf("u.c = %c\n\n", u_SSP.c_SSP);

    printf("Addresses Of Members Of Union Are : \n\n");
    printf("u.i = %p\n\n", &u_SSP.i_SSP);
    printf("u.f = %p\n\n", &u_SSP.f_SSP);
    printf("u.d = %p\n\n", &u_SSP.d_SSP);
    printf("u.c = %p\n\n", &u_SSP.c_SSP);
    
    printf("MyUnion u = %p\n\n", &u_SSP);

    return(0);
}