#include <stdio.h>
#define NUM_ELEMENTS 10
int main(void)
{
    //variable declarations 
    int iArray[NUM_ELEMENTS];
    int i, num, sum = 0;
    //code
    printf("\n\n");
    // *** ARRAY ELEMENTS INPUT ***
    printf("Enter Integer Elememts For Array : \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &num);
        iArray[i] = num;
    }
    // *** SEPARATING OUT EVEN NUMBERS FROM ARRAY ELEMENTS ***
    printf("\n\n");
    printf("Even Numbers Amongst The Array Elements Are : \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        if ((iArray[i] % 2) == 0)
        printf("%d\n", iArray[i]);
    }
    // *** SEPARATING OUT ODD NUMBERS FROM ARRAY ELEMENTS ***
    printf("\n\n");
    printf("Odd Numbers Amongst The Array Elements Are : \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        if ((iArray[i] % 2) != 0)
        printf("%d\n", iArray[i]);
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
78
9

10
1


Even Numbers Amongst The Array Elements Are :

2
4
6
78
10


Odd Numbers Amongst The Array Elements Are :

1
3
5
9
1
*/