#include<stdio.h>

int main(void)
{
    int num_SSP;
    int *ptr_SSP = NULL;
    int *copy_ptr_SSP=NULL;

    // code
    num_SSP=5;
    ptr_SSP=&num_SSP;

    printf("\n\n");
    printf(" ***** Before copy_ptr = ptr ***** \n\n");
    printf(" num        =%d\n",num_SSP);
    printf(" &num       =%p\n",&num_SSP);
    printf(" *(&num)    =%d\n",*(&num_SSP));
    printf(" ptr        =%p\n",ptr_SSP);
    printf(" *ptr       =%d\n",*ptr_SSP);

    // 'ptr' is an integer pointer variable...that is it can hold the address of any integer variable only
    // 'copy_ptr' is another integer pointer variable 
    //  if 'ptr = &num'...'ptr' will contain the adresss of innteger variable 'num'
    //  if 'ptr' is assigned to 'copy_ptr','copy_ptr' will  also contain the address of integer variable 'num'
    //  Hence, now both 'ptr' and 'copy_ptr' will point to 'num'

    copy_ptr_SSP = ptr_SSP;     //copy_ptr=ptr=&num

    printf("\n\n");
    printf(" ***** After copy_ptr = ptr ***** \n\n");
    printf(" num        =%d\n",num_SSP);
    printf(" &num       =%p\n",&num_SSP);
    printf(" *(&num)    =%d\n",*(&num_SSP));
    printf(" ptr        =%p\n",ptr_SSP);
    printf(" *ptr       =%d\n",*ptr_SSP);
    printf(" copy_ptr   =%p\n",copy_ptr_SSP);
    printf(" *copy_ptr  =%d\n",*copy_ptr_SSP);
    return 0;
}