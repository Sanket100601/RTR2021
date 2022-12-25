#include <stdio.h>
#include <stdarg.h>

#define NUM_TO_BE_FOUND 3
#define NUM_ELEMENTS 10
int main(void)
{
    // function prototypes
    void FindNumber(int, int, ...);
 
    // code
    printf("\n\n");
 
    FindNumber(NUM_TO_BE_FOUND, NUM_ELEMENTS, 3, 5, 9, 2, 3, 6, 9, 3, 1, 3);
 
    return(0);
}

void FindNumber(int num_to_be_found, int num, ...) // VARIADIC FUNCTION
{
    // variable declarations
    int count_SSP = 0;
    int n_SSP;
    va_list numbers_list_SSP;
 
    // code
    va_start(numbers_list_SSP, num);
 
    while(num)
    {
        n_SSP = va_arg(numbers_list_SSP, int);
        if(n_SSP == num_to_be_found)
        count_SSP++;
        num--;
    }
 
    if(count_SSP == 0)
    printf("Number %d Could Not Be Found !!!\n\n", num_to_be_found);
    else
    printf("Number %d Found %d Times !!!\n\n", num_to_be_found, count_SSP);
 
    va_end(numbers_list_SSP);
}