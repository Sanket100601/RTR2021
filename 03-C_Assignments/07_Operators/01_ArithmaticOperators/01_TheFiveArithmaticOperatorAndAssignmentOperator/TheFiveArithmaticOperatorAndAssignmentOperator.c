#include<stdio.h>

int main(void)
{
    //variable declaration
    int a_SSP;
    int b_SSP;
    int result_SSP;

    //code
    printf("\n\n");
    printf("Enter a number : ");
    scanf("%d",&a_SSP);

    printf("\n\n");
    printf("Enter another number : ");
    scanf("%d",&b_SSP);

    printf("\n\n");

    // *** The Following Are The 5 Arithmetic Operators +, -, *, / and % ***
	// *** Also, The Resultants Of The Arithmetic Operations In All The Below Five Cases Have Been Assigned To The Variable 'result' Using the Assignment Operator (=) ***

    result_SSP=a_SSP+b_SSP;
    printf("Addition of a_SSP=%d and b_SSP=%d gives %d.\n",a_SSP,b_SSP,result_SSP);

    result_SSP=a_SSP-b_SSP;
    printf("Subtraction of a_SSP=%d and b_SSP=%d gives %d.\n",a_SSP,b_SSP,result_SSP);

    result_SSP=a_SSP*b_SSP;
    printf("Multiplication of a_SSP=%d and b_SSP=%d gives %d.\n",a_SSP,b_SSP,result_SSP);

    result_SSP=a_SSP/b_SSP;
    printf("Division of a_SSP=%d and b_SSP=%d gives Quotient %d.\n",a_SSP,b_SSP,result_SSP);

    result_SSP=a_SSP%b_SSP;
    printf("Division of a_SSP=%d and b_SSP=%d gives Remainder %d.\n",a_SSP,b_SSP,result_SSP);

    printf("\n\n");

    return 0;
}
/*
Enter a number : 21


Enter another number : 23


Addition of a_SSP=21 and b_SSP=23 gives 44.
Subtraction of a_SSP=21 and b_SSP=23 gives -2.
Multiplication of a_SSP=21 and b_SSP=23 gives 483.
Division of a_SSP=21 and b_SSP=23 gives Quotient 0.
Division of a_SSP=21 and b_SSP=23 gives Remainder 21.
*/