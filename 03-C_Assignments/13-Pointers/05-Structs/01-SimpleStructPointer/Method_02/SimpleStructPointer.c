#include <stdio.h>
#include<stdlib.h>

// DEFINING STRUCT
struct MyData
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
};

int main(void)
{
    //variable declarations
    int i_size_SSP;
    int f_size_SSP;
    int d_size_SSP;
    int struct_MyData_size_SSP;
    int pointer_to_struct_MyData_size_SSP;
    struct MyData *pData_SSP = NULL;

    //code
    printf("\n\n");
    pData_SSP = (struct MyData *)malloc(sizeof(struct MyData));
    if (pData_SSP == NULL)
    {
        printf("FAILED TO ALLOCATE MEMORY TO 'sturct MyData' !!! EXITTING NOW ...\n\n");
        exit(0);
    }
    else
        printf("SUCCESSFULLY ALLOCATED MEMORY TO 'sturct MyData' !!!\n\n");
    
    //Assigning Data Values To The Data Members Of 'struct MyData'
    pData_SSP->i_SSP = 30;
    pData_SSP->f_SSP = 11.45f;
    pData_SSP->d_SSP = 1.2995;
   
    //Displaying Values Of The Data Members Of 'struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n", pData_SSP->i_SSP);
    printf("f = %f\n", pData_SSP->f_SSP);
    printf("d = %lf\n", pData_SSP->d_SSP);
   
    //Calculating Sizes (In Bytes) Of The Data Members Of 'struct MyData'
    i_size_SSP = sizeof(pData_SSP->i_SSP);
    f_size_SSP = sizeof(pData_SSP->f_SSP);
    d_size_SSP = sizeof(pData_SSP->d_SSP);
   
    //Displaying Sizes (In Bytes) Of The Data Members Of 'struct MyData'
    printf("\n\n");
    printf("SIZES (in bytes) OF DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("Size of 'i' = %d bytes\n", i_size_SSP);
    printf("Size of 'f' = %d bytes\n", f_size_SSP);
    printf("Size of 'd' = %d bytes\n", d_size_SSP);
   
    //Calculating Size (In Bytes) Of the entire 'struct Mydata'
    struct_MyData_size_SSP = sizeof(struct MyData); 
    pointer_to_struct_MyData_size_SSP = sizeof(struct MyData *);
   
    //Displaying Sizes (In Bytes) Of the entire 'struct Mydata'
    printf("\n\n");
    printf("Size of 'struct MyData' : %d bytes\n\n", struct_MyData_size_SSP);
    printf("Size of pointer to 'struct MyData' : %d bytes\n\n", pointer_to_struct_MyData_size_SSP);
    if (pData_SSP)
    {
        free(pData_SSP);
        pData_SSP = NULL;
        printf("MEMORY ALLOCATED TO 'struct MyData' HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    return(0);
}
