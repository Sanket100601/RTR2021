#include<stdio.h>   //contains  the declaration of printf()
int main(int argc,char*argv[],char*envp[])
{
    //function prototype/declaration/signature
    void MyAddition(void);

    //code
    MyAddition();   //functionn call
    return 0;
}

//User defined function : method of  definition 1
///No Return Value,No Parameters

void MyAddition(void)   //function definition
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

    printf("\n\n ");
    printf("Sum Of %d And %d = %d\n\n",a_SSP,b_SSP,sum_SSP);
}
/*
Enter Integer Value For 'A' : 20


Enter Integer Value For 'B' : 12


 Sum Of 20 And 12 = 32
 */