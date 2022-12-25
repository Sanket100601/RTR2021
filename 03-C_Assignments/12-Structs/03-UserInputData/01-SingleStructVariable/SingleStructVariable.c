#include <stdio.h>
#include <conio.h>
struct MyData
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
    char ch_SSP;
};

int main(void)
{
    //variable declarations
    struct MyData data_SSP; //declaring a single struct variable

    //code
    //User Input For Values Of Data Members Of 'struct MyData'
    printf("\n\n");
    printf("Enter Integer Value For Data Member 'i' Of 'struct MyData' : \n");
    scanf("%d", &data_SSP.i_SSP);
    printf("Enter Floating-Point Value For Data Member 'f' Of 'struct MyData' : \n");
    scanf("%f", &data_SSP.f_SSP);
    printf("Enter 'Double' Value For Data Member 'd' Of 'struct MyData' : \n");
    scanf("%lf", &data_SSP.d_SSP);
    printf("Enter Character Value For Data Member 'c' Of 'struct MyData' : \n");
    data_SSP.ch_SSP = getch();

    //Display Values Of Data Members Of 'struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n", data_SSP.i_SSP);
    printf("f = %f\n", data_SSP.f_SSP);
    printf("d = %lf\n", data_SSP.d_SSP);
    printf("c = %c\n\n", data_SSP.ch_SSP);

    return(0);
}
