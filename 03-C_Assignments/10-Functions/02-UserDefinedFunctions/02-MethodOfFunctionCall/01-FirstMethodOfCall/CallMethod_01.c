#include <stdio.h> //'stdio.h' contains declaration of 'printf()'
// ****** USER DEFINED FUNCTIONS : METHOD OF CALLING FUNCTION 1 ******
// ****** CALLING ALL FUNCTIONS IN main() DIRECTLY ******
// Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters
//(int argc, char *argv[], char *envp[])
int main(int argc, char *argv[], char *envp[])
{
    //function prototypes OR declarations
    void MyAddition(void);
    int MySubtraction(void);    
    void MyMultiplication(int, int);
    int MyDivision(int, int);
    //variable declarations
    int result_subtraction_SSP;
    int a_multiplication_SSP, b_multiplication_SSP;
    int a_division_SSP, b_division_SSP, result_division_SSP;
    //code
    // *** ADDITION ***
    MyAddition(); //function call
    // *** SUBTRACTION ***
    result_subtraction_SSP = MySubtraction(); //function call
    printf("\n\n");
    printf("Subtraction Yields Result = %d\n", result_subtraction_SSP);
    // *** MULTIPLICATION ***
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Multiplication : ");
    scanf("%d", &a_multiplication_SSP);
    printf("\n\n");
    printf("Enter Integer Value For 'B' For Multiplication : ");
    scanf("%d", &b_multiplication_SSP);
    MyMultiplication(a_multiplication_SSP, b_multiplication_SSP); //function call
    // *** DIVISION ***
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Division : ");
    scanf("%d", &a_division_SSP);
    printf("\n\n");
    printf("Enter Integer Value For 'B' For Division : ");
    scanf("%d", &b_division_SSP);
    result_division_SSP = MyDivision(a_division_SSP, b_division_SSP); //function call
    printf("\n\n");
    printf("Division Of %d and %d Gives = %d (Quotient)\n", a_division_SSP,b_division_SSP, result_division_SSP);
    printf("\n\n");
    return(0);
}
// *** Function Definition Of MyAddition() ******
void MyAddition(void) //function definition
{
    //variable declarations : local variables to MyAddition()
    int a_SSP, b_SSP, sum_SSP;
    //code
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Addition : ");
    scanf("%d", &a_SSP);
    printf("\n\n");
    printf("Enter Integer Value For 'B' For Addition : ");
    scanf("%d", &b_SSP);
    sum_SSP = a_SSP + b_SSP;
    printf("\n\n");
    printf("Sum Of %d And %d = %d\n\n", a_SSP, b_SSP, sum_SSP);
}
// *** Function Definition Of MySubtraction() ******
int MySubtraction(void) //function definition
{
    //variable declarations : local variables to MySubtraction()
    int a_SSP, b_SSP, subtraction_SSP;
    //code
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Subtraction : ");
    scanf("%d", &a_SSP);
    printf("\n\n");
    printf("Enter Integer Value For 'B' For Subtraction : ");
    scanf("%d", &b_SSP);
    subtraction_SSP = a_SSP - b_SSP;
    return(subtraction_SSP);
}
// *** Function Definition Of MyMultiplication() ******
void MyMultiplication(int a_SSP, int b_SSP) //function definition
{
    //variable declarations : local variables to MyMultiplication()
    int multiplication_SSP;
    //code
    multiplication_SSP = a_SSP * b_SSP;
    printf("\n\n");
    printf("Multiplication Of %d And %d = %d\n\n", a_SSP, b_SSP, multiplication_SSP);
}
    // *** Function Definition Of MyDivision() ******
    int MyDivision(int a_SSP, int b_SSP) //function definition
{
    //variable declarations : local variables to MyDivision()
    int division_quotient_SSP;
    //code
    if (a_SSP > b_SSP)
        division_quotient_SSP = a_SSP / b_SSP;
    else
        division_quotient_SSP = b_SSP / a_SSP;
    return(division_quotient_SSP);
}
/*
Enter Integer Value For 'A' For Addition : 21


Enter Integer Value For 'B' For Addition : '35


Sum Of 21 And 4199136 = 4199157



Enter Integer Value For 'A' For Subtraction :

Enter Integer Value For 'B' For Subtraction :

Subtraction Yields Result = -4199115


Enter Integer Value For 'A' For Multiplication :

Enter Integer Value For 'B' For Multiplication :

Multiplication Of 13119288 And 4201440 = -1708902144



Enter Integer Value For 'A' For Division :

Enter Integer Value For 'B' For Division :

Division Of 4201531 and 6422356 Gives = 1 (Quotient)
*/