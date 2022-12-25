#include<stdio.h>

int main()
{
    //function prototype
    void PrintBinaryFormOfNumber(unsigned int);

    //variable declaration
    unsigned int a_SSP;
    unsigned int b_SSP;
    unsigned int result_SSP;

    //code
    printf("\n\n");
    printf("Enter an integer: ");
    scanf("%u",&a_SSP);

    printf("\n\n");
    printf("Enter another integer: ");
    scanf("%u",&b_SSP);

    printf("\n\n\n");
    result_SSP = a_SSP ^ b_SSP;
    printf("Bitwise XOR-ing Of \nA = %d (Decimal) and B = %d (Decimal) gives result %d (Decimal).\n\n", a_SSP, b_SSP, result_SSP);
    PrintBinaryFormOfNumber(a_SSP);
    PrintBinaryFormOfNumber(b_SSP);
    PrintBinaryFormOfNumber(result_SSP);

    return(0);
}

void PrintBinaryFormOfNumber(unsigned int decimal_number_SSP)
{
    //variable declarations
    unsigned int quotient_SSP, remainder_SSP;
    unsigned int num_SSP;
    unsigned int binary_array_SSP[8];
    int i_SSP;
    //code
    for (i_SSP = 0; i_SSP < 8; i_SSP++)
        binary_array_SSP[i_SSP] = 0;
        printf("The Binary Form Of The Decimal Integer %d Is\t=\t", decimal_number_SSP);
        num_SSP = decimal_number_SSP;
        i_SSP = 7;
    while (num_SSP != 0)
    {
        quotient_SSP = num_SSP / 2;
        remainder_SSP = num_SSP % 2;
        binary_array_SSP[i_SSP] = remainder_SSP;
        num_SSP = quotient_SSP;
        i_SSP--;
    }
    for (i_SSP = 0; i_SSP < 8; i_SSP++)
        printf("%u", binary_array_SSP[i_SSP]);
    printf("\n\n");
}