#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    //function declarations
    void MultiplyArrayElementsByNumber(int *, int, int);

    //variable declaration
    int *iArray_SSP = NULL;
    int num_elements_SSP;
    int i_SSP, num_SSP;

    //code
    printf("\n\n");
    printf("Enter How Many Elements You Want In The Integer Array : ");
    scanf("%d", &num_elements_SSP);
    iArray_SSP = (int *)malloc(num_elements_SSP * sizeof(int));
    if (iArray_SSP == NULL)
    {
        printf("MEMORY ALLOCATION TO 'iArray' HAS FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
    printf("\n\n");
    printf("Enter %d Elements For The Integer Array : \n\n", num_elements_SSP);
    for (i_SSP = 0; i_SSP < num_elements_SSP; i_SSP++)
        scanf("%d", &iArray_SSP[i_SSP]);
    
    // ****** ONE ******
    printf("\n\n");
    printf("Array Before Passing To Function MultiplyArrayElementsByNumber() : \n\n");
    for (i_SSP = 0; i_SSP < num_elements_SSP; i_SSP++)
    printf("iArray[%d] = %d\n", i_SSP, iArray_SSP[i_SSP]);
    printf("\n\n");
    printf("Enter The Number By Which You Want To Multiply Each Array Element : ");
    scanf("%d", &num_SSP);
    MultiplyArrayElementsByNumber(iArray_SSP, num_elements_SSP, num_SSP);
    printf("\n\n");
    printf("Array Returned By Function MultiplyArrayElementsByNumber() : \n\n");
    for (i_SSP = 0; i_SSP < num_elements_SSP; i_SSP++)
    printf("iArray[%d] = %d\n", i_SSP, iArray_SSP[i_SSP]);
    if (iArray_SSP)
    {
        free(iArray_SSP);
        iArray_SSP = NULL;
        printf("\n\n");
        printf("MEMORY ALLOCATED TO 'iArray' HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    return(0);
}
void MultiplyArrayElementsByNumber(int *arr, int iNumElements, int n)
{
    //variable declarations
    int i_SSP;
    //code
    for (i_SSP = 0; i_SSP < iNumElements; i_SSP++)
    arr[i_SSP] = arr[i_SSP] * n;
}