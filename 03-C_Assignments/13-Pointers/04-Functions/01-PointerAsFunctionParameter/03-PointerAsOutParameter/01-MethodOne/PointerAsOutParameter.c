#include<stdio.h>
int main(void)
{
    // functionn declaration
    void MathematicalOperations(int,int,int*,int*,int*,int*,int*);

    // variable declaration
    int a_SSP;
    int b_SSP;
    int answer_sum_SSP;
    int answer_difference_SSP;
    int answer_product_SSP;
    int answer_quotient_SSP;
    int answer_remainder_SSP;

    // code
    printf("\n\n");
    printf("Enter the Value Of 'A' : ");
    scanf("%d",&a_SSP);

    printf("\n\n");
    printf("Enter The Value Of 'B' : ");
    scanf("%d",&b_SSP);

    // PASSING ADDRESSES TO FUNCTION ... FUNCTION WILL FILL THEM UP WITH VALUES ... HENCE, THEY GO INTO THE FUNCTION AS ADDRESS PARAMETERS AND COME OUT OF THE FUNCTION FILLED WITH VALID VALUES
    // THUS, (&answer_sum, &answer_difference, &answer_product, &answer_quotient, &answer_remainder) ARE CALLED "OUT PARAMETERS" OR "PARAMETERIZED RETURN VALUES" ... RETURN VALUES OF FUNCTIONS COMING VIA PARAMETERS
    // HENCE, ALTHOUGH EACH FUNCTION HAS ONLY ONE RETURN VALUE, USING THE CONCEPT OF "PARAMETERIZED RETURN VALUES", OUR FUNCTION "MathematicalOperations()" HAS GIVEN US 5 RETURN VALUES !!!

    MathematicalOperations(a_SSP,b_SSP,&answer_sum_SSP,&answer_difference_SSP,&answer_product_SSP,&answer_quotient_SSP,&answer_remainder_SSP);

    printf("\n\n");
    printf("***** RESULTS ****\n\n");
    printf("Sum = %d\n\n",answer_sum_SSP);
    printf("Difference = %d\n\n",answer_difference_SSP);
    printf("Product = %d\n\n",answer_product_SSP);
    printf("Quotient = %d\n\n",answer_quotient_SSP);
    printf("Remainder = %d\n\n",answer_remainder_SSP);

    return 0;
}

void MathematicalOperations(int x, int y, int *sum, int *difference, int *product, int *quotient, int *remainder)
{
    // code 
    *sum=x+y;
    *difference=x-y;
    *product=x*y;
    *quotient=x/y;
    *remainder=x%y;
}