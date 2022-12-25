#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    //function declarations
    void MathematicalOperations(int, int, int *, int *, int *, int *, int *);
    
    //variable declaration
    int a_SSP;  
    int b_SSP;
    int *answer_sum_SSP = NULL;
    int *answer_difference_SSP = NULL;
    int *answer_product_SSP = NULL;
    int *answer_quotient_SSP = NULL;
    int *answer_remainder_SSP = NULL;

    //code
    printf("\n\n");
    printf("Enter Value Of 'A' : ");
    scanf("%d", &a_SSP);
    printf("\n\n");
    printf("Enter Value Of 'B' : ");
    scanf("%d", &b_SSP);

    // PASSING ADDRESSES TO FUNCTION ... FUNCTION WILL FILL THEM UP WITH VALUES ... HENCE, THEY GO INTO THE FUNCTION AS ADDRESS PARAMETERS AND COME OUT OF THE FUNCTION FILLED WITH VALID VALUES
    // THUS, (&answer_sum, &answer_difference, &answer_product, &answer_quotient, &answer_remainder) ARE CALLED "OUT PARAMETERS" OR "PARAMETERIZED RETURN VALUES" ... RETURN VALUES OF FUNCTIONS COMING VIA PARAMETERS
    // HENCE, ALTHOUGH EACH FUNCTION HAS ONLY ONE RETURN VALUE, USING THE CONCEPT OF "PARAMETERIZED RETURN VALUES", OUR FUNCTION "MathematicalOperations()" HAS GIVEN US 5 RETURN VALUES !!!
    answer_sum_SSP = (int *)malloc(1 * sizeof(int));
    if (answer_sum_SSP == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_sum'. Exitting Now...\n\n");
        exit(0);
    }
    answer_difference_SSP = (int *)malloc(1 * sizeof(int));
    if (answer_difference_SSP == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_difference'. Exitting Now...\n\n");
        exit(0);
    }
    answer_product_SSP = (int *)malloc(1 * sizeof(int));
    if (answer_product_SSP == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_product'. Exitting Now...\n\n");
        exit(0);
    }
    answer_quotient_SSP = (int *)malloc(1 * sizeof(int));
    if (answer_quotient_SSP == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_quotient'. Exitting Now...\n\n");
        exit(0);
    }
    answer_remainder_SSP = (int *)malloc(1 * sizeof(int));
    if (answer_remainder_SSP == NULL)
    {   
        printf("Could Not Allocate Memory For 'answer_remainder'. Exitting Now...\n\n");
        exit(0);
    }
    
    MathematicalOperations(a_SSP, b_SSP, answer_sum_SSP, answer_difference_SSP, answer_product_SSP, answer_quotient_SSP, answer_remainder_SSP);
    
    printf("\n\n");
    printf("****** RESULTS ****** \n\n");   
    printf("Sum = %d\n\n", *answer_sum_SSP);
    printf("Difference = %d\n\n", *answer_difference_SSP);
    printf("Product = %d\n\n", *answer_product_SSP);
    printf("Quotient = %d\n\n", *answer_quotient_SSP);
    printf("Remainder = %d\n\n", *answer_remainder_SSP);
    if (answer_remainder_SSP)
    {
        free(answer_remainder_SSP);
        answer_remainder_SSP = NULL;
        printf("Memory Allocated For 'answer_remainder' Successfully Freed !!!\n\n");
    }
    if (answer_quotient_SSP)
    {
        free(answer_quotient_SSP);
        answer_quotient_SSP = NULL;
        printf("Memory Allocated For 'answer_quotient' Successfully Freed !!!\n\n");
    }
    if (answer_product_SSP)
    {
        free(answer_product_SSP);
        answer_product_SSP = NULL;
        printf("Memory Allocated For 'answer_product' Successfully Freed !!!\n\n");
    }
    if (answer_difference_SSP)
    {
        free(answer_difference_SSP);
        answer_difference_SSP = NULL;
        printf("Memory Allocated For 'answer_difference' Successfully Freed !!!\n\n");
    }
    if (answer_sum_SSP)
    {
        free(answer_sum_SSP);
        answer_sum_SSP = NULL;
        printf("Memory Allocated For 'answer_sum' Successfully Freed !!!\n\n");
    }
    return(0);
}

void MathematicalOperations(int x, int y, int *sum, int *difference, int *product, int *quotient, int *remainder) 
{
    //code
    *sum = x + y; // Value at address 'sum' = (x + y)
    *difference = x - y; // Value at address 'difference' = (x - y)
    *product = x * y; // Value at address 'product' = (x * y)
    *quotient = x / y; // Value at address 'quotient' = (x / y)
    *remainder = x % y; // Value at address 'remainder' = (x % y)
}