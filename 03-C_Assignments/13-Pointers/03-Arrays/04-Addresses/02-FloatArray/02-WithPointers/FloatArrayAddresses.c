#include <stdio.h>
int main(void)
{
    //variable declarations
    float fArray_SSP[10];
    float *ptr_fArray_SSP = NULL;
    int i_SSP;
    //code
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    fArray_SSP[i_SSP] = (float)(i_SSP + 1) * 1.5;
    // *** NAME OF ANY ARRAY IS ITS BASE ADDRESS ***
    // *** HENCE, 'fArray' IS THE BASE ADDRESS OF ARRAY fArray[] OR 'fArray' IS THE ADDRESS OF ELEMENT fArray[0] ***
    // *** ASSIGNING BASE ADDRESS OF ARRAY 'fArray[]' TO INTEGER POINTER 'ptr_iArray'
    ptr_fArray_SSP = fArray_SSP; // ptr_fArray = &fArray[0];

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("fArray[%d] = %f\n", i_SSP, *(ptr_fArray_SSP + i_SSP));

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("fArray[%d] = %f \t \t Address = %p\n", i_SSP, *(ptr_fArray_SSP + i_SSP), (ptr_fArray_SSP + i_SSP));
    
    printf("\n\n");
    
    return(0);
}