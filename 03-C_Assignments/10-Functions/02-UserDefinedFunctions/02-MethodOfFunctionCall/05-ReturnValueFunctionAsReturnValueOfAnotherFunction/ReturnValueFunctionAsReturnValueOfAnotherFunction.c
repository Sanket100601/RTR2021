#include<stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    //functionn prototype or declaration
    int MyAddition(int,int);

    //variable declaration
    int r_SSP;
    int num_01_SSP,num_02_SSP;
   
    //code
    num_01_SSP=10;
    num_02_SSP=20;
   
    printf("\n\n");
    printf("%d+%d",num_01_SSP,num_02_SSP,MyAddition(num_01_SSP,num_02_SSP));
    printf("\n\n");

    return 0;
}

// Function definition of MyAddition()
int MyAddition(int a_SSP,int b_SSP)   //function definiiton
{
    //function   prototype
    int Add(int,int);

    //code 
    return(Add(a_SSP,b_SSP));   //return value of function Add() as return value of fuction MyAddition()
}
int Add(int x_SSP,int y_SSP)
{
    //code
    return(x_SSP,y_SSP);
}
// 10+20