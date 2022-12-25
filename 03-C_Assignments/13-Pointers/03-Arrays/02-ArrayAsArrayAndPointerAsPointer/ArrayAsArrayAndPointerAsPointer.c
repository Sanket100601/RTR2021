#include <stdio.h>
int main(void)
{
    //variable declarations
    int iArray_SSP[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 }; //Integer Array
    int *ptr_iArray_SSP = NULL; //Integer Pointer
    //code
    // ###### USING ARRAY NAME AS A ARRAY i.e : Value Of xth Element Of iArray : iArray[x] AND Address Of xth Element Of iArray : &iArray[x] ######
    printf("\n\n");
    printf("*** USING ARRAY NAME AS A ARRAY i.e : Value Of xth Element Of iArray : iArray[x] AND Address Of xth Element Of iArray : &iArray[x] ***\n\n");
    printf("Integer Array Elements And Their Addresses : \n\n");
    printf("iArray[0] = %d \t \t At Address &iArray[0] : %p\n", iArray_SSP[0], &iArray_SSP[0]);
    printf("iArray[1] = %d \t \t At Address &iArray[1] : %p\n", iArray_SSP[1], &iArray_SSP[1]);
    printf("iArray[2] = %d \t \t At Address &iArray[2] : %p\n", iArray_SSP[2], &iArray_SSP[2]);
    printf("iArray[3] = %d \t \t At Address &iArray[3] : %p\n", iArray_SSP[3], &iArray_SSP[3]);
    printf("iArray[4] = %d \t \t At Address &iArray[4] : %p\n", iArray_SSP[4], &iArray_SSP[4]);
    printf("iArray[5] = %d \t \t At Address &iArray[5] : %p\n", iArray_SSP[5], &iArray_SSP[5]);
    printf("iArray[6] = %d \t \t At Address &iArray[6] : %p\n", iArray_SSP[6], &iArray_SSP[6]);
    printf("iArray[7] = %d \t \t At Address &iArray[7] : %p\n", iArray_SSP[7], &iArray_SSP[7]);
    printf("iArray[8] = %d \t \t At Address &iArray[8] : %p\n", iArray_SSP[8], &iArray_SSP[8]);
    printf("iArray[9] = %d \t At Address &iArray[9] : %p\n", iArray_SSP[9], &iArray_SSP[9]);

    // Assinging base address of integer array  'iArray' to integer pointer 'ptr_Array'
    ptr_iArray_SSP=iArray_SSP;   // Same As...ptr_iArray = &iArray[0]
    
    // ###### USING POINTER AS POINTER i.e : Value Of xth Element Of iArray : *(ptr_iArray + x) AND Address Of xth Element Of iArray : (ptr_iArray + x) ######

    printf("\n\n");
    printf("*** USING POINTER AS POINTER i.e : Value Of xth Element Of iArray : *(ptr_iArray + x) AND Address Of xth Element Of iArray : (ptr_iArray + x) ***\n\n");
    printf("Integer Array Elements And Their Addresses : \n\n");
    printf("*(ptr_iArray + 0) = %d \t \t At Address (ptr_iArray + 0) : %p\n", *(ptr_iArray_SSP + 0), (ptr_iArray_SSP + 0));
    printf("*(ptr_iArray + 1) = %d \t \t At Address (ptr_iArray + 1) : %p\n", *(ptr_iArray_SSP + 1), (ptr_iArray_SSP + 1));
    printf("*(ptr_iArray + 2) = %d \t \t At Address (ptr_iArray + 2) : %p\n", *(ptr_iArray_SSP + 2), (ptr_iArray_SSP + 2));
    printf("*(ptr_iArray + 3) = %d \t \t At Address (ptr_iArray + 3) : %p\n", *(ptr_iArray_SSP + 3), (ptr_iArray_SSP + 3));
    printf("*(ptr_iArray + 4) = %d \t \t At Address (ptr_iArray + 4) : %p\n", *(ptr_iArray_SSP + 4), (ptr_iArray_SSP + 4));
    printf("*(ptr_iArray + 5) = %d \t \t At Address (ptr_iArray + 5) : %p\n", *(ptr_iArray_SSP + 5), (ptr_iArray_SSP + 5));
    printf("*(ptr_iArray + 6) = %d \t \t At Address (ptr_iArray + 6) : %p\n", *(ptr_iArray_SSP + 6), (ptr_iArray_SSP + 6));
    printf("*(ptr_iArray + 7) = %d \t \t At Address (ptr_iArray + 7) : %p\n", *(ptr_iArray_SSP + 7), (ptr_iArray_SSP + 7));
    printf("*(ptr_iArray + 8) = %d \t \t At Address (ptr_iArray + 8) : %p\n", *(ptr_iArray_SSP + 8), (ptr_iArray_SSP + 8));
    printf("*(ptr_iArray + 9) = %d \t At Address (ptr_iArray + 9) : %p\n", *(ptr_iArray_SSP + 9), (ptr_iArray_SSP + 9));
    
    return(0);
}
