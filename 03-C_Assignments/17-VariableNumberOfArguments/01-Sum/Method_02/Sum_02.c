#include <stdio.h>
#include <stdarg.h>

int main(void)
{
    // function prototypes
    int CalculateSum(int, ...);
 
    // variable declarations
    int answer_SSP;
 
    // code
    printf("\n\n");
 
    answer_SSP = CalculateSum(5, 10, 20, 30, 40, 50);
    printf("Answer = %d\n\n", answer_SSP);
 
    answer_SSP = CalculateSum(10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    printf("Answer = %d\n\n", answer_SSP);
    answer_SSP = CalculateSum(0);
    printf("Answer = %d\n\n", answer_SSP);
    return(0);
}

int CalculateSum(int num, ...) // VARIADIC FUNCTION
{
 
    // function prototype
    int va_CalculateSum(int, va_list);
 
    // variable declarations
    int sum_SSP = 0;
    va_list numbers_list;
 
    // code
    va_start(numbers_list, num);
 
    sum_SSP = va_CalculateSum(num, numbers_list);
 
    va_end(numbers_list);
    
    return(sum_SSP);
}

int va_CalculateSum(int num, va_list list)
{
    // variable declaration
    int n_SSP;
    int sum_total_SSP = 0;
 
    // code
 
    while(num)
    {
        n_SSP = va_arg(list, int);
        sum_total_SSP = sum_total_SSP + n_SSP;
        num--;
    }
 
    return(sum_total_SSP);
}