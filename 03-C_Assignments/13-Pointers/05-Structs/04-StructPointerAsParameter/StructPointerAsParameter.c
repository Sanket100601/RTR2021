#include <stdio.h>

// DEFINING STRUCT
struct MyData
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
};

int main(void)
{
    //function prototypes
    void ChangeValues(struct MyData *);
    
    //variable declarations
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
    ChangeValues(pData_SSP);
  
    //Displaying Values Of The Data Members Of 'struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n", pData_SSP->i_SSP);
    printf("f = %f\n", pData_SSP->f_SSP);
    printf("d = %lf\n", pData_SSP->d_SSP);
    if (pData_SSP)
    {
        free(pData_SSP);
        pData_SSP = NULL;
        printf("MEMORY ALLOCATED TO 'struct MyData' HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    return(0);
}

void ChangeValues(struct MyData *pParam_Data)
{
    //code
    pParam_Data->i_SSP = 9;
    pParam_Data->f_SSP = 8.2f;
    pParam_Data->d_SSP = 6.1998;
    
    // CAN ALSO DO THIS AS ...
    /*
    (*pParam_Data).i = 9;
    (*pParam_Data).f = 8.2f;
    (*pParam_Data).d = 6.1998;
    */
}
