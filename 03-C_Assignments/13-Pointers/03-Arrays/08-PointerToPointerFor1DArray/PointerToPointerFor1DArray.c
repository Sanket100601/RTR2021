#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    void MyAlloc(int **ptr, unsigned int numberOfElements);

    // variable Declrations
    int *piArray_SSP = NULL;
    unsigned int num_elements_SSP;
    int i_SSP;

    //code
    printf("\n\n");
    printf("How Many Elements You Want In Integer Array ?\n\n");
    scanf("%d",&num_elements_SSP);

    printf("\n\n");
    MyAlloc(&piArray_SSP, num_elements_SSP);

    printf("Enter %u Elements To Fill Up Your Integer Array : \n\n",num_elements_SSP);
    for(i_SSP = 0; i_SSP < num_elements_SSP; i_SSP++)
    {
        scanf("%d", &piArray_SSP[i_SSP]);
    }

    printf("\n\n");
    printf("The %u Elements Entered By You In The Integer Array :\n\n", num_elements_SSP);
    for(i_SSP = 0; i_SSP < num_elements_SSP; i_SSP++)
    {
        printf("%u\n", piArray_SSP[i_SSP]);
    }

    printf("\n\n");
    if(piArray_SSP)
    {
        free(piArray_SSP);
        piArray_SSP = NULL;
        printf("Memory Allocated Has Been Successfully Freed !!!");
    }
    return 0;
}

void MyAlloc(int **ptr, unsigned int numberOfElements)
{
    // code
    *ptr = (int *)malloc(numberOfElements *sizeof(int));
    if(*ptr == NULL)
    {
        printf("Could Not Allocate Memory !!! Exiting Now...\n\n");
        exit(0);
    }
    printf("MyAlloc() has Succesfully Allocated %lu Bytes For Integer Array !!!\n\n", (numberOfElements * sizeof(int)));
}
