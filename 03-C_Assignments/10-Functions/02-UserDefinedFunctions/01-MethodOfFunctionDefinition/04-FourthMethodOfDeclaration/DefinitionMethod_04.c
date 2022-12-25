#include<stdio.h>   //contains  the declaration of printf()
int main(int argc,char*argv[],char*envp[])
{
    //function prototype/declaration/signature
    int MyAddition(int,int);

    //variable declaration : local Variable to main
   int a_SSP,b_SSP,result_SSP;

    //code
    printf("\n\n");
    printf("Enter Integer Value For 'A' : ");
    scanf("%d",&a_SSP);

    printf("\n\n");
    printf("Enter Integer Value For 'B' : ");
    scanf("%d",&b_SSP);

    result_SSP=MyAddition(a_SSP,b_SSP);    //functional call
    printf("\n\n");
    printf("Sum Of %d and %d = %d\n\n",a_SSP,b_SSP,result_SSP);

    return(0);
}

//User defined function : method of  definition 4
//VALID(int) Return Value,valid Parameters (int,int)

int MyAddition(int a_SSP,int b_SSP)   //function definition
{
    //variable declaration  : local variables to  MyAddition()
    int sum_SSP;

    //code
    sum_SSP=a_SSP+b_SSP;
   return(sum_SSP);
}
/*
Enter Integer Value For 'A' : 23


Enter Integer Value For 'B' : 12


Sum Of 23 and 12 = 35
*/