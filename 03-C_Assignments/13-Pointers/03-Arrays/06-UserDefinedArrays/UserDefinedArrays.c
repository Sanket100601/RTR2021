#include <stdio.h>
#include <stdlib.h>

#define INT_SIZE sizeof(int)
#define FLOAT_SIZE sizeof(float)
#define DOUBLE_SIZE sizeof(double)
#define CHAR_SIZE sizeof(char)

int main(void)
{
    //variable declarations
    int *ptr_iArray_SSP = NULL;
    unsigned int intArrayLength_SSP = 0;

    float *ptr_fArray_SSP = NULL;
    unsigned int floatArrayLength_SSP = 0;

    double *ptr_dArray_SSP = NULL;
    unsigned int doubleArrayLength_SSP = 0;

    char *ptr_cArray_SSP = NULL;
    unsigned int charArrayLength_SSP = 0;

    int i_SSP;
    
    //code

    // ****** INTEGER ARRAY *******
    printf("\n\n");
    printf("Enter The Number Of Elements You Want In The Integer Array : ");
    scanf("%u", &intArrayLength_SSP);
    ptr_iArray_SSP = (int *)malloc(INT_SIZE * intArrayLength_SSP);
    if (ptr_iArray_SSP == NULL)
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR INTEGER ARRAY FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR INTEGER ARRAY SUCCEEDED !!!\n\n");
    }
    printf("\n\n");
    printf("Enter The %d Integer Elements To Fill Up The Integer Array : \n\n", intArrayLength_SSP);
    for (i_SSP = 0; i_SSP < intArrayLength_SSP; i_SSP++)
        scanf("%d", (ptr_iArray_SSP + i_SSP));

    // ****** FLOAT ARRAY *******
    printf("\n\n");
    printf("Enter The Number Of Elements You Want In The 'float' Array : ");
    scanf("%u", &floatArrayLength_SSP);

    ptr_fArray_SSP = (float *)malloc(FLOAT_SIZE * floatArrayLength_SSP);
    if (ptr_fArray_SSP == NULL)
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR FLOATING-POINT ARRAY FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR FLOATING-POINT ARRAY SUCCEEDED !!!\n\n");
    }
    printf("\n\n");
    printf("Enter The %d Floating-Point Elements To Fill Up The 'float' Array : \n\n", floatArrayLength_SSP);
    for (i_SSP = 0; i_SSP < floatArrayLength_SSP; i_SSP++)
        scanf("%f", (ptr_fArray_SSP + i_SSP));

    // ****** DOUBLE ARRAY *******
    printf("\n\n");
    printf("Enter The Number Of Elements You Want In The 'double' Array : ");
    scanf("%u", &doubleArrayLength_SSP);

    ptr_dArray_SSP = (double *)malloc(DOUBLE_SIZE * doubleArrayLength_SSP);
    if (ptr_dArray_SSP == NULL)
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR 'DOUBLE' ARRAY FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR 'DOUBLE' ARRAY SUCCEEDED !!!\n\n");
    }
    printf("\n\n");
    printf("Enter The %d Double Elements To Fill Up The 'double' Array : \n\n", doubleArrayLength_SSP);
    for (i_SSP = 0; i_SSP < doubleArrayLength_SSP; i_SSP++)
        scanf("%lf", (ptr_dArray_SSP + i_SSP));

    // ****** CHAR ARRAY *******
    printf("\n\n");
    printf("Enter The Number Of Elements You Want In The Character Array : ");
    scanf("%u", &charArrayLength_SSP);
    ptr_cArray_SSP = (char *)malloc(CHAR_SIZE * charArrayLength_SSP);
    if (ptr_cArray_SSP == NULL)
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR CHARACTER ARRAY FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR CHARACTER ARRAY SUCCEEDED !!!\n\n");
    }
    printf("\n\n");
    printf("Enter The %d Character Elements To Fill Up The Character Array : \n\n", charArrayLength_SSP);
    for (i_SSP = 0; i_SSP < charArrayLength_SSP; i_SSP++)
    {
        *(ptr_cArray_SSP + i_SSP) = getch();
        printf("%c\n", *(ptr_cArray_SSP + i_SSP));
    }

    // ********** DISPLAY OF ARRAYS **********
    
    // ****** INTEGER ARRAY ******
    printf("\n\n");
    printf("The Integer Array Entered By You And Consisting Of %d Elements Is As Follows : \n\n", intArrayLength_SSP);
    for (i_SSP = 0; i_SSP < intArrayLength_SSP; i_SSP++)
        printf(" %d \t \t At Address : %p\n", *(ptr_iArray_SSP + i_SSP), (ptr_iArray_SSP + i_SSP));
    
    // ****** FLOAT ARRAY ******
    printf("\n\n");
    printf("The Float Array Entered By You And Consisting Of %d Elements Is As Follows : \n\n", floatArrayLength_SSP);
    for (i_SSP = 0; i_SSP < floatArrayLength_SSP; i_SSP++)
        printf(" %f \t \t At Address : %p\n", *(ptr_fArray_SSP + i_SSP), (ptr_fArray_SSP + i_SSP));

    // ****** DOUBLE ARRAY ******
    printf("\n\n");
    printf("The Double Array Entered By You And Consisting Of %d Elements Is As Follows : \n\n", doubleArrayLength_SSP);
    for (i_SSP = 0; i_SSP < doubleArrayLength_SSP; i_SSP++)
        printf(" %lf \t \t At Address : %p\n", *(ptr_dArray_SSP + i_SSP), (ptr_dArray_SSP + i_SSP));
    
    // ****** CHARACTER ARRAY ******
    printf("\n\n");
    printf("The Character Array Entered By You And Consisting Of %d Elements Is As Follows : \n\n", charArrayLength_SSP);
    for (i_SSP = 0; i_SSP < charArrayLength_SSP; i_SSP++)
    printf(" %c \t \t At Address : %p\n", *(ptr_cArray_SSP + i_SSP), (ptr_cArray_SSP + i_SSP));
    
    // ****** FREEING MEMORY OCCUPIED BY ARRAYS (IN REVERSE ORDER OF ALLOCATION) ******
    if (ptr_cArray_SSP)
    {
        free(ptr_cArray_SSP);
        ptr_cArray_SSP = NULL;
        printf("\n\n");
        printf("MEMORY OCCUPIED BY CHARACTER ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    if (ptr_dArray_SSP)
    {
        free(ptr_dArray_SSP);
        ptr_dArray_SSP = NULL;
        printf("\n\n");
        printf("MEMORY OCCUPIED BY 'DOUBLE' ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    if (ptr_fArray_SSP)
    {
        free(ptr_fArray_SSP);
        ptr_fArray_SSP = NULL;
        printf("\n\n");
        printf("MEMORY OCCUPIED BY FLOATING-POINT ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    if (ptr_iArray_SSP)
    {
        free(ptr_iArray_SSP);
        ptr_iArray_SSP = NULL;
        printf("\n\n");
        printf("MEMORY OCCUPIED BY INTEGER ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    return(0);
}
