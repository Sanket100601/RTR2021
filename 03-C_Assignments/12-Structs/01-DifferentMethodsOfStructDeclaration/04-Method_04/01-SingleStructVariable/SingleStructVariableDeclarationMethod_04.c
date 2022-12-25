#include<stdio.h>

// Defining Struct
struct MyData
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
};

struct MyData data; // Defining Single Variable For of type 'struct MyData' Globally

int main(void)
{
    // varible declaration
    struct MyData data; // Defining Single Variable For of type 'struct MyData' Locally
    int i_size_SSP;
    float f_size_SSP;
    double d_size_SSP;
    int struct_MyData_size_SSP;

    // code
    // Assinging Data Value To The Data Member Of 'struct MyData'
    data.i_SSP=30;
    data.f_SSP=11.45f;
    data.d_SSP=1.2995;

    // Displaying The  Values of DAta Members of 'struct MyData'
    printf("\n\n");
    printf("Data Members Of 'struct MyData' are : \n\n");
    printf("i=%d\n",data.i_SSP);
    printf("f=%f\n",data.f_SSP);
    printf("d=%lf\n",data.d_SSP);

    // Calculating the sizes (in Bytes) of Data members Of "struct MyData"
    i_size_SSP=sizeof(data.i_SSP);
    f_size_SSP=sizeof(data.f_SSP);
    d_size_SSP=sizeof(data.d_SSP);

    // Displaying the sizes (In Bytes) Of The Data Members of 'struct MyData'
    printf("\n\n");
    printf("SIZES (in bytes) OF DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("Size of 'i' = %d bytes\n", i_size_SSP);
    printf("Size of 'f' = %d bytes\n", f_size_SSP);
    printf("Size of 'd' = %d bytes\n", d_size_SSP);

    //Calculating Size (In Bytes) Of the entire 'struct Mydata'
    struct_MyData_size_SSP=sizeof(struct MyData);

    //Displaying Size (in Bytes) of the entire 'struct MyData'
    printf("\n\n");
    printf("size Of 'struct MyData' : %d bytes\n\n",struct_MyData_size_SSP);

    return 0;
}
