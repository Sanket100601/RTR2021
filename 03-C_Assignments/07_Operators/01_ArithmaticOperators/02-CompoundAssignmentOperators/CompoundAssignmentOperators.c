#include <stdio.h>

int main(void)
{
	//variable declarations
	int a_SSP;
	int b_SSP;
	int x_SSP;

	//code
	printf("\n\n");
	printf("Enter A Number : ");
	scanf("%d", &a_SSP);

	printf("\n\n");
	printf("Enter Another Number : ");
	scanf("%d", &b_SSP);

	printf("\n\n");

	//Since, In All The Following 5 Cases, The Operand on The Left 'a_SSP' Is Getting Repeated Immeiately On The Right (e.g : a_SSP = a_SSP + b_SSP or a_SSP = a_SSP -b_SSP),
	//We Are Using Compound Assignment Operators +=, -=, *=, /= and %=

	//Since, 'a' Will Be Assigned The Value Of (a_SSP + b_SSP) At The Expression (a_SSP += b_SSP), We Must Save The Original Value Of 'a_SSP' To Another Variable (x)
	x_SSP = a_SSP;
	a_SSP += b_SSP; // a_SSP = a + b_SSP
	printf("Addition Of A = %d And B = %d Gives %d.\n", x_SSP, b_SSP, a_SSP);

	//Value Of 'a_SSP' Altered In The Above Expression Is Used Here...
	//Since, 'a_SSP' Will Be Assigned The Value Of (a_SSP - b_SSP) At The Expression (a_SSP -= b_SSP), We Must Save The Original Value Of 'a_SSP' To Another Variable (x)
	x_SSP = a_SSP;
	a_SSP -= b_SSP; // a_SSP = a_SSP - b_SSP
	printf("Subtraction Of A = %d And B = %d Gives %d.\n", x_SSP, b_SSP, a_SSP);

	//Value Of 'a_SSP' Altered In The Above Expression Is Used Here...
	//Since, 'a_SSP' Will Be Assigned The Value Of (a * b) At The Expression (a_SSP *= b_SSP), We Must Save The Original Value Of 'a_SSP' To Another Variable (x)
	x_SSP = a_SSP;
	a_SSP *= b_SSP; // a_SSP = a_SSP * b_SSP
	printf("Multiplication Of A_SSP = %d And B_SSP = %d Gives %d.\n", x_SSP, b_SSP, a_SSP);

	//Value Of 'a_SSP' Altered In The Above Expression Is Used Here...
	//Since, 'a_SSP' Will Be Assigned The Value Of (a_SSP / b_SSP) At The Expression (a_SSP /= b_SSP), We Must Save The Original Value Of 'a_SSP' To Another Variable (x)
	x_SSP = a_SSP;
	a_SSP /= b_SSP; // a_SSP = a_SSP / b_SSP 
	printf("Division Of A_SSP = %d And B_SSP = %d Gives Quotient %d.\n", x_SSP, b_SSP, a_SSP);

	//Value Of 'a_SSP' Altered In The Above Expression Is Used Here...
	//Since, 'a_SSP' Will Be Assigned The Value Of (a_SSP % b_SSP) At The Expression (a_SSP %= b_SSP), We Must Save The Original Value Of 'a_SSP' To Another Variable (x)
	x_SSP = a_SSP;
	a_SSP %= b_SSP; // a_SSP = a_SSP % b_SSP
	printf("Division Of A_SSP = %d And B_SSP = %d Gives Remainder %d.\n", x_SSP, b_SSP, a_SSP);

	printf("\n\n");

	return(0);
}
/*
Enter A Number : 34


Enter Another Number : 12


Addition Of A = 34 And B = 12 Gives 46.
Subtraction Of A = 46 And B = 12 Gives 34.
Multiplication Of A_SSP = 34 And B_SSP = 12 Gives 408.
Division Of A_SSP = 408 And B_SSP = 12 Gives Quotient 34.
Division Of A_SSP = 34 And B_SSP = 12 Gives Remainder 10.
*/