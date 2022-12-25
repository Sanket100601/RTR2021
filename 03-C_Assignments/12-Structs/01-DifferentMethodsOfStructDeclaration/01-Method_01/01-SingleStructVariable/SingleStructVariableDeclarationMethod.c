#include<stdio.h>

//Defining Struct
struct MyData
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
    char c_SSP;
} data; //Declaring a single struct variable of type 'struct MyData' globally ...

int main(void)
{
    // variable Declration
    int i_size_SSP;
    int f_size_SSP;
    int d_size_SSP;
    int struct_myData_size_SSP;

    //code
    //Assigning Data Values To The DAta Members of 'struct MyData'
    data.i_SSP=30;
    data.f_SSP=11.45f;
    data.d_SSP=1.2995; 

    //Displaying Values Of The Data Members Of 'struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' Are : \n\n");
    printf("i = %d\n",data.i_SSP);
    printf("f = %f\n",data.f_SSP);
    printf("d = %lf\f",data.d_SSP);

    //Calculating Sizes (In Bytes) Of The Data Members Of 'struct MyData'
    i_size_SSP = sizeof(data.i_SSP);    
    f_size_SSP = sizeof(data.f_SSP);
    d_size_SSP = sizeof(data.d_SSP);

    //Displaying Sizes (In Bytes) Of The Data Members Of 'struct MyData'
    printf("\n\n");
    printf("SIZES (in bytes) OF DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("Size of 'i' = %d bytes\n", i_size_SSP);
    printf("Size of 'f' = %d bytes\n", f_size_SSP);
    printf("Size of 'd' = %d bytes\n", d_size_SSP);

    //Calculating Size (In Bytes) Of the entire 'struct Mydata'
    struct_myData_size_SSP = sizeof(struct MyData); //can also give struct name -> sizeof(MyData)
   
    //Displaying Sizes (In Bytes) Of the entire 'struct Mydata'
    printf("\n\n");
    printf("Size of 'struct MyData' : %d bytes\n\n", struct_myData_size_SSP);

    return 0;
}