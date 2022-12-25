#include <stdio.h>
#include <stdlib.h>

// DEFINING STRUCT
struct MyData
{
    int *ptr_i_SSP;
    int i_SSP;
    float *ptr_f_SSP;
    float f_SSP;
    double *ptr_d_SSP;
    double d_SSP;
};

int main(void)
{
    //variable declarations
    struct MyData *pData_SSP = NULL;
    
    //code
    printf("\n\n");
    pData_SSP = (struct MyData *)malloc(sizeof(struct MyData));
    if (pData_SSP == NULL)
    {
        printf("FAILED TO ALLOCATE MEMORY TO 'struct MyData' !!! EXITTING NOW ... \n\n");
        exit(0);
    }
    else
        printf("SUCCESSFULLY ALLOCATED MEMORY TO 'struct MyData' !!!\n\n");
    pData_SSP->i_SSP = 9;
    pData_SSP->ptr_i_SSP = &(pData_SSP->i_SSP);
    pData_SSP->f_SSP = 11.45f;
    pData_SSP->ptr_f_SSP = &(pData_SSP->f_SSP);
    pData_SSP->d_SSP = 30.121995;
    pData_SSP->ptr_d_SSP = &(pData_SSP->d_SSP);
    printf("\n\n");
    printf("i = %d\n", *(pData_SSP->ptr_i_SSP));
    printf("Adress Of 'i' = %p\n", pData_SSP->ptr_i_SSP);
    printf("\n\n");
    printf("f = %f\n", *(pData_SSP->ptr_f_SSP));
    printf("Adress Of 'f' = %p\n", pData_SSP->ptr_f_SSP);
    printf("\n\n");
    printf("d = %lf\n", *(pData_SSP->ptr_d_SSP));
    printf("Adress Of 'd' = %p\n", pData_SSP->ptr_d_SSP);
    if (pData_SSP)
    {
        free(pData_SSP);
        pData_SSP = NULL;
        printf("MEMORY ALLOCATED TO 'struct MyData' HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    return(0);
}
