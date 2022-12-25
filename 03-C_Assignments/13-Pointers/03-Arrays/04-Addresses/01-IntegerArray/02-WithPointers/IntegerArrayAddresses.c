#include <stdio.h>
int main(void)
{
    //variable declarations
    int iArray_SSP[10];
    int *ptr_iArray_SSP = NULL;
    int i_SSP;
    //code
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    iArray_SSP[i_SSP] = (i_SSP + 1) * 3;
    // *** NAME OF ANY ARRAY IS ITS BASE ADDRESS ***
    // *** HENCE, 'iArray' IS THE BASE ADDRESS OF ARRAY iArray[] OR 'iArray' IS THE ADDRESS OF ELEMENT iArray[0] ***
    // *** ASSIGNING BASE ADDRESS OF ARRAY 'iArray[]' TO INTEGER POINTER 'ptr_iArray'
    ptr_iArray_SSP = iArray_SSP; // ptr_iArray = &iArray[0];

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("iArray[%d] = %d\n", i_SSP, *(ptr_iArray_SSP + i_SSP));

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("iArray[%d] = %d \t \t Address = %p\n", i_SSP, *(ptr_iArray_SSP + i_SSP), (ptr_iArray_SSP + i_SSP));
    
    printf("\n\n");
    
    return(0);
}