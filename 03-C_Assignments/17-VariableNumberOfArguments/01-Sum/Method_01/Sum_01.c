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
    // variable declarations
    int sum_total_SSP = 0;
    int n_SSP;
 
    va_list numbers_list_SSP;
 
    // code
    va_start(numbers_list_SSP, num);
 
    while(num)
    {
        n_SSP = va_arg(numbers_list_SSP, int);
        sum_total_SSP = sum_total_SSP + n_SSP;
        num--;
    }
 
    va_end(numbers_list_SSP);
    
    return(sum_total_SSP);
}
