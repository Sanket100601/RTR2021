#include <stdio.h>
#define NUM_ELEMENTS 10
int main(void)
{
    //variable declarations 
    int iArray_SSP[NUM_ELEMENTS];
    int i_SSP, num_SSP, j_SSP, count_SSP = 0;
    //code
    printf("\n\n");
    // *** ARRAY ELEMENTS INPUT ***
    printf("Enter Integer Elememts For Array : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ELEMENTS; i_SSP++)
    {
        scanf("%d", &num_SSP);
        // If 'num' is negative ( < 0 ), then convert it to positive (multiply by -1)
        if (num_SSP < 0)
            num_SSP = -1 * num_SSP;
        iArray_SSP[i_SSP] = num_SSP;
    }
    // *** PRINTING ENTIRE ARRAY ***    
    printf("\n\n"); 
    printf("Array Elements Are : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ELEMENTS; i_SSP++)
    printf("%d\n", iArray_SSP[i_SSP]);
    // *** SEPARATING OUT EVEN NUMBERS FROM ARRAY ELEMENTS ***
    printf("\n\n");
    printf("Prime Numbers Amongst The Array Elements Are : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ELEMENTS; i_SSP++)
    {
        for (j_SSP = 1; j_SSP <= iArray_SSP[i_SSP]; j_SSP++)
        {
            if ((iArray_SSP[i_SSP] % j_SSP) == 0)
            count_SSP++;
        }
    // NUMBER 1 IS NEITHER A PRIME NUMBER NOR A CONSONANT
    // IF A NUMBER IS PRIME, IT IS ONLY DIVISIBLE BY 1 AND ITSELF.
    // HENCE, IF A NUMBER IS PRIME, THE VALUE OF 'count' WILL BE EXACTLY 2.
    // IF THE VALUE OF 'count' IS GREATER THAN 2, THE NUMBER IS DIVISIBLE BY NUMBERS OTHER THAN 1 AND ITSLEF AND HENCE, IT IS NOT PRIME
    // THE VALUE OF 'count' WILL BE 1 ONLY IF iArray[i] IS 1.
    if (count_SSP == 2)
    printf("%d\n", iArray_SSP[i_SSP]);
    count_SSP = 0; //RESET 'count' TO 0 FOR CHECKING THE NEXT NUMBER...
    }
    return(0);
}
/*
Enter Integer Elememts For Array : 

1
2
3
4
5
6
7
8
9
0


Array Elements Are :

1
2
3
4
5
6
7
8
9
0


Prime Numbers Amongst The Array Elements Are :

2
3
5
7
*/