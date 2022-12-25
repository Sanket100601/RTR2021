#include <stdio.h>
int main(void)
{
    //variable declarations
    int a_SSP;
    int b_SSP;
    int c_SSP;
    int result_SSP;
    //code
    printf("\n\n");
    printf("Enter First Integer : ");
    scanf("%d", &a_SSP);
    printf("\n\n");
    printf("Enter Second Integer : ");
    scanf("%d", &b_SSP);
    printf("\n\n");
    printf("Enter Third Integer : ");
    scanf("%d", &c_SSP);
    printf("\n\n");
    printf("If Answer = 0, It Is 'FALSE'.\n");
    printf("If Answer = 1, It Is 'TRUE'.\n\n");
    result_SSP = (a_SSP <= b_SSP) && (b_SSP != c_SSP);
    printf("LOGICAL AND (&&) : Answer is TRUE (1) If And Only If BOTH Conditions Are True. The Answer is FALSE (0), If Any One Or Both Conditions Are False.\n\n");
    printf("A_SSP = %d Is Less Than Or Equal To B_SSP = %d AND B_SSP = %d Is NOT Equal To C_SSP = %d \t Answer = %d\n\n", a_SSP, b_SSP, b_SSP, c_SSP, result_SSP);
    result_SSP = (b_SSP >= a_SSP) || (a_SSP == c_SSP);
    printf("LOGICAL OR (||) : Answer is FALSE (0) If And Only If BOTH Conditions Are False. The Answer is TRUE (1), If Any One Or Both Conditions Are True.\n\n");
    printf("Either B_SSP = %d Is Greater Than Or Equal To A_SSP = %d OR A_SSP = %d Is Equal To C_SSP = %d \t Answer_SSP = %d\n\n", b_SSP, a_SSP, a_SSP, c_SSP, result_SSP);
    result_SSP = !a_SSP;
    printf("A_SSP = %d And Using Logical NOT (!) Operator on A Gives Result = %d\n\n", a_SSP, result_SSP);
    result_SSP = !b_SSP;
    printf("B_SSP = %d And Using Logical NOT (!) Operator on B_SSP Gives Result_SSP = %d\n\n", b_SSP, result_SSP);
    result_SSP = !c_SSP;
    printf("C_SSP = %d And Using Logical NOT (!) Operator on C_SSP Gives Result = %d\n\n", c_SSP, result_SSP);
    result_SSP = (!(a_SSP <= b_SSP) && !(b_SSP != c_SSP));
    printf("Using Logical NOT (!) On (a_SSP <= b_SSP) And Also On (b_SSP != c_SSP) And then ANDing Them Afterwards Gives Result = %d\n", result_SSP);
    printf("\n\n");
    result_SSP = !((b_SSP >= a_SSP) || (a_SSP == c_SSP));
    printf("Using Logical NOT (!) On Entire Logical Expression (b_SSP >= a_SSP) || (a_SSP ==c_SSP) Gives Result_SSP = %d\n", result_SSP);
    printf("\n\n");

    return(0);
}