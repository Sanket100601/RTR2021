#include <stdio.h>
int main(void)
{
    //DEFINING STRUCT
    struct MyData
    {
        int i_SSP;
        float f_SSP;
        double d_SSP;
    } data_SSP; //Declaring a single struct variable of type 'struct MyData' locally...
    
    //variable declarations
    int i_size_SSP;
    int f_size_SSP;
    int d_size_SSP;
    int struct_MyData_size_SSP;
    
    //code
    //Assigning Data Values To The Data Members Of 'struct MyData'
    data_SSP.i_SSP = 30;
    data_SSP.f_SSP = 11.45f;
    data_SSP.d_SSP = 1.2995;
    
    //Displaying Values Of The Data Members Of 'struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n", data_SSP.i_SSP);
    printf("f = %f\n", data_SSP.f_SSP);
    printf("d = %lf\n", data_SSP.d_SSP);
    
    //Calculating Sizes (In Bytes) Of The Data Members Of 'struct MyData'
    i_size_SSP = sizeof(data_SSP.i_SSP);
    f_size_SSP = sizeof(data_SSP.f_SSP);
    d_size_SSP = sizeof(data_SSP.d_SSP);
    
    //Displaying Sizes (In Bytes) Of The Data Members Of 'struct MyData'
    printf("\n\n");
    printf("SIZES (in bytes) OF DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("Size of 'i' = %d bytes\n", i_size_SSP);
    printf("Size of 'f' = %d bytes\n", f_size_SSP);
    printf("Size of 'd' = %d bytes\n", d_size_SSP);
    
    //Calculating Size (In Bytes) Of the entire 'struct Mydata'
    struct_MyData_size_SSP = sizeof(struct MyData); //can also give struct name -> sizeof(MyData)
    
    //Displaying Sizes (In Bytes) Of the entire 'struct Mydata'
    printf("\n\n");
    printf("Size of 'struct MyData' : %d bytes\n\n", struct_MyData_size_SSP);
    return(0);
}
