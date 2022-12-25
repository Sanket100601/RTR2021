#include<stdio.h>

int main(void)
{
    int num_SSP;
    int *ptr_SSP=NULL;
    int ans;

    // code
    num_SSP = 5;
    ptr_SSP=&num_SSP;

    printf("\n\n");
    printf(" num =      %d\n",num_SSP);
    printf(" &num =     %d\n",&num_SSP);
    printf(" *(&num) =   %d\n",*(&num_SSP));
    printf(" ptr =      %d\n",ptr_SSP);
    printf(" *ptr =     %d\n",*ptr_SSP);

    printf("\n\n");

    printf("Answer Of  (ptr + 10)   = %p\n",(ptr_SSP+10));

    printf("Answer Of  *(ptr + 10)  = %d\n",*(ptr_SSP+10));

    printf("Answer Of  (*ptr + 10)  = %d\n",(*ptr_SSP+10));

    ++*ptr_SSP;
    printf("Answer Of  ++*ptr       = %d\n",*ptr_SSP);

    *ptr_SSP++;
    printf("Answer Of  *ptr++         = %d\n",*ptr_SSP);

    ptr_SSP=&num_SSP;
    (*ptr_SSP)++;
    printf("Answer Of *(ptr)++      = %d\n",*ptr_SSP);

    return 0;
}