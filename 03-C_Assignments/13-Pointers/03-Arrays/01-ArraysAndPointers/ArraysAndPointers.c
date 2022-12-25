#include <stdio.h>
int main(void)
{
    //variable declarations
    int iArray_SSP[] = { 12, 24, 36, 48, 60, 72, 84, 96, 108, 120 };
    float fArray_SSP[] = { 9.8f, 8.7f, 7.6f, 6.5f, 5.4f };
    double dArray_SSP[] = { 1.222222, 2.333333, 3.444444 };
    char cArray_SSP[] = { 'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P', '\0' };
    //code  
    printf("\n\n");
    printf("Integer Array Elements And The Addresses They Occupy Are As Follows : \n\n");
    printf("iArray[0] = %d \t At Address : %p\n", *(iArray_SSP + 0), (iArray_SSP + 0));
    printf("iArray[1] = %d \t At Address : %p\n", *(iArray_SSP + 1), (iArray_SSP + 1));
    printf("iArray[2] = %d \t At Address : %p\n", *(iArray_SSP + 2), (iArray_SSP + 2));
    printf("iArray[3] = %d \t At Address : %p\n", *(iArray_SSP + 3), (iArray_SSP + 3));
    printf("iArray[4] = %d \t At Address : %p\n", *(iArray_SSP + 4), (iArray_SSP + 4));
    printf("iArray[5] = %d \t At Address : %p\n", *(iArray_SSP + 5), (iArray_SSP + 5));
    printf("iArray[6] = %d \t At Address : %p\n", *(iArray_SSP + 6), (iArray_SSP + 6));
    printf("iArray[7] = %d \t At Address : %p\n", *(iArray_SSP + 7), (iArray_SSP + 7));
    printf("iArray[8] = %d \t At Address : %p\n", *(iArray_SSP + 8), (iArray_SSP + 8));
    printf("iArray[9] = %d \t At Address : %p\n", *(iArray_SSP + 9), (iArray_SSP + 9));
    
    printf("\n\n");
    printf("Float Array Elements And The Addresses They Occupy Are As Follows : \n\n");
    printf("fArray[0] = %f \t At Address : %p\n", *(fArray_SSP + 0), (fArray_SSP + 0));
    printf("fArray[1] = %f \t At Address : %p\n", *(fArray_SSP + 1), (fArray_SSP + 1));
    printf("fArray[2] = %f \t At Address : %p\n", *(fArray_SSP + 2), (fArray_SSP + 2));
    printf("fArray[3] = %f \t At Address : %p\n", *(fArray_SSP + 3), (fArray_SSP + 3));
    printf("fArray[4] = %f \t At Address : %p\n", *(fArray_SSP + 4), (fArray_SSP + 4));

    printf("\n\n");
    printf("Double Array Elements And The Addresses They Occupy Are As Follows : \n\n");
    printf("dArray[0] = %lf \t At Address : %p\n", *(dArray_SSP + 0), (dArray_SSP + 0));
    printf("dArray[1] = %lf \t At Address : %p\n", *(dArray_SSP + 1), (dArray_SSP + 1));
    printf("dArray[2] = %lf \t At Address : %p\n", *(dArray_SSP + 2), (dArray_SSP + 2));
    
    printf("\n\n");
    printf("Character Array Elements And The Addresses They Occupy Are As Follows : \n\n");
    printf("cArray[0] = %c \t At Address : %p\n", *(cArray_SSP + 0), (cArray_SSP + 0));
    printf("cArray[1] = %c \t At Address : %p\n", *(cArray_SSP + 1), (cArray_SSP + 1));
    printf("cArray[2] = %c \t At Address : %p\n", *(cArray_SSP + 2), (cArray_SSP + 2));
    printf("cArray[3] = %c \t At Address : %p\n", *(cArray_SSP + 3), (cArray_SSP + 3));
    printf("cArray[4] = %c \t At Address : %p\n", *(cArray_SSP + 4), (cArray_SSP + 4));
    printf("cArray[5] = %c \t At Address : %p\n", *(cArray_SSP + 5), (cArray_SSP + 5));
    printf("cArray[6] = %c \t At Address : %p\n", *(cArray_SSP + 6), (cArray_SSP + 6));
    printf("cArray[7] = %c \t At Address : %p\n", *(cArray_SSP + 7), (cArray_SSP + 7));
    printf("cArray[8] = %c \t At Address : %p\n", *(cArray_SSP + 8), (cArray_SSP + 8));
    printf("cArray[9] = %c \t At Address : %p\n", *(cArray_SSP + 9), (cArray_SSP + 9));
    printf("cArray[10] = %c \t At Address : %p\n", *(cArray_SSP + 10), (cArray_SSP + 10));
    printf("cArray[11] = %c \t At Address : %p\n", *(cArray_SSP + 11), (cArray_SSP + 11));
    printf("cArray[12] = %c \t At Address : %p\n", *(cArray_SSP + 12), (cArray_SSP + 12));
    printf("cArray[13] = %c \t At Address : %p\n", *(cArray_SSP + 13), (cArray_SSP + 13));
    
    return(0);
}