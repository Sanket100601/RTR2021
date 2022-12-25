#include<stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    //functionn prototype or declaration
    int MyAddition(int,int);

    //variable declaration
    int r_SSP;
    int num_01_SSP,num_02_SSP;
    int num_03_SSP,num_04_SSP;

    //code
    num_01_SSP=10;
    num_02_SSP=20;
    num_03_SSP=30;
    num_04_SSP=40;

    r_SSP=MyAddition(MyAddition(num_01_SSP,num_02_SSP),MyAddition(num_03_SSP,num_04_SSP));  //return value of MyAddition()is sent as parameterto another call to MyAddition()

    printf("\n\n");
    printf("%d+%d+%d+%d",num_01_SSP,num_02_SSP,num_03_SSP,num_04_SSP,r_SSP);
    printf("\n\n");

    return 0;
}

//Function  Definition Of MyAddition()
int MyAddition(int a_SSP,int b_SSP) //function definition
{
    //variable  declaration
    int sum_SSP;

    //code 
    sum_SSP=a_SSP+b_SSP;
    return(sum_SSP);
}
// 10+20+30+40
