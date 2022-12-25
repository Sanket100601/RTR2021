#include<stdio.h>   //contains  the declaration of printf()
int main(int argc,char*argv[],char*envp[])
{
    //function prototype/declaration/signature
    int MyAddition(void);

    //variable declaration : local Variable to main
    int result_SSP;

    //code
    result_SSP=MyAddition();   //functionn call

    printf("\n\n");
    printf("Sum = %d\n\n",result_SSP);
    return 0;
}

//User defined function : method of  definition 2
//VALID(int) Return Value,No Parameters

int MyAddition(void)   //function definition
{
    //variable declaration  : local variables to  MyAddition()
    int a_SSP,b_SSP,sum_SSP;

    //code
    printf("\n\n");
    printf("Enter Integer Value For 'A' : ");
    scanf("%d",&a_SSP);

    printf("\n\n");
    printf("Enter Integer Value For 'B' : ");
    scanf("%d",&b_SSP);

    sum_SSP=a_SSP+b_SSP;

    return(sum_SSP);
}
/*
Enter Integer Value For 'A' : 29


Enter Integer Value For 'B' : 34


Sum = 63
*/