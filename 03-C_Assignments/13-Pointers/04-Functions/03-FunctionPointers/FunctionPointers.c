#include<stdio.h>

int main(void)
{
    // function Declaration
    int AddIntegers(int, int);
    int SubtractIntegers(int, int);
    float AddFloats(float,float);

    // variable Declarions
    typedef int (*AddIntsFnPtr)(int, int);
    AddIntsFnPtr ptrAddTwoIntegers = NULL;
    AddIntsFnPtr ptrFunc = NULL;

    typedef float (*AddFloatFunctPtr)(float, float);
    AddFloatFunctPtr ptrAddTwoFloats = NULL;

    int iAnswer_SSP = 0;
    float fAnswer_SSP = 0.0f;

    // code
    ptrAddTwoIntegers = AddIntegers;
    iAnswer_SSP = ptrAddTwoIntegers(9, 30);
    printf("\n\n");
    printf("Sum Of integers = %d\n\n", iAnswer_SSP);

    ptrFunc = SubtractIntegers;
    iAnswer_SSP = ptrFunc(9, 30);
    printf("\n\n");
    printf("Subtraction of Integers = %d\n\n", iAnswer_SSP);

    ptrAddTwoFloats = AddFloats;
    fAnswer_SSP = ptrAddTwoFloats(11.45f, 8.2f);
    printf("\n\n");
    printf("Sum Of Two Floating Point numbers = %f\n\n", fAnswer_SSP);


    return 0;
}

int AddIntegers(int a, int b)
{
    //  variable declaration
    int c_SSP;

    // code
    c_SSP = a + b;

    return c_SSP;
}

int SubtractIntegers(int a, int b)
{
    //  variable declaration
    int c_SSP;

    // code
    if(a > b)
        c_SSP = a - b;
    else
        c_SSP = b - a;
    
    return c_SSP;
}

float AddFloats(float f_num1, float f_num2)
{
    // variables Declaration
    float ans_SSP;

    // code 
    ans_SSP = f_num1 + f_num2;

    return ans_SSP;
}
