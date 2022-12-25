#include <stdio.h>
int main(void)
{
    //variable declarations
    char cArray_SSP[10];
    char *ptr_cArray_SSP = NULL;
    int i_SSP;
    //code
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        cArray_SSP[i_SSP] = (char)(i_SSP + 65);
    // *** NAME OF ANY ARRAY IS ITS BASE ADDRESS ***
    // *** HENCE, 'cArray' IS THE BASE ADDRESS OF ARRAY cArray[] OR 'cArray' IS THE ADDRESS OF ELEMENT cArray[0] ***
    // *** ASSIGNING BASE ADDRESS OF ARRAY 'cArray[]' TO INTEGER POINTER 'ptr_cArray'
    ptr_cArray_SSP = cArray_SSP; // ptr_cArray = &cArray[0];

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("cArray[%d] = %c\n", i_SSP, *(ptr_cArray_SSP + i_SSP));

    printf("\n\n");
    printf("Elements Of The Integer Array : \n\n");
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("cArray[%d] = %c \t \t Address = %p\n", i_SSP, *(ptr_cArray_SSP + i_SSP), (ptr_cArray_SSP + i_SSP));
    
    printf("\n\n");
    
    return(0);
}