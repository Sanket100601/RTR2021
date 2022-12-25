#include <stdio.h>
int main(void)
{
    //variable declarations
    double dArray_SSP[10];
    double *ptr_dArray_SSP = NULL;
    int i_SSP;
    //code
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        dArray_SSP[i_SSP] = (float)(i_SSP + 1) * 1.33333333;
    // *** NAME OF ANY ARRAY IS ITS BASE ADDRESS ***
    // *** HENCE, 'fArray' IS THE BASE ADDRESS OF ARRAY fArray[] OR 'fArray' IS THE ADDRESS OF ELEMENT fArray[0] ***
    // *** ASSIGNING BASE ADDRESS OF ARRAY 'fArray[]' TO INTEGER POINTER 'ptr_iArray'
    ptr_dArray_SSP = dArray_SSP; // ptr_fArray = &fArray[0];

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("fArray[%d] = %lf\n", i_SSP, *(ptr_dArray_SSP + i_SSP));

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("fArray[%d] = %f \t \t Address = %p\n", i_SSP, *(ptr_dArray_SSP + i_SSP), (ptr_dArray_SSP + i_SSP));
    
    printf("\n\n");
    
    return(0);
}