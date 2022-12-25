#include<stdio.h>   //'stdio.h' contains Declaration of 'printf()'
#include<ctype.h>   //'ctype.h' containns declaration of 'atoi()'
#include<stdlib.h>  //'stdlib.h' contains declaration of 'exit()'

int main(int argc,char*argv[],char*envp[])
{
    //variable declarion
    int i_SSP;
    int num_SSP;
    int sum_SSP=0;

    //code 
    if (argc == 1)
    {
        /* code */
        printf("\n\n");
        printf("No Number Given For addition !!! Exitting Now...\n\n");
        printf("Usage  : CommandLineArgumentsApplication <first Number> <second number>...\n\n");
        exit(0);
    }
    //Thi programs add All command line arguments given in integer from only and outputs the sum 
    //due to use of atoi(),All command line arguments of types other than 'int' are ignored
    printf("\n\n");
    printf("sum of all innteger command line arguments is : \n\n");
    for(i_SSP=1;i_SSP<argc;i_SSP++) //loop starts from i=1 because, i=0 will result in argv[i_SSP] = argv[0] which is the  name of program itself
    {
        num_SSP=atoi(argv[i_SSP]);
        sum_SSP=sum_SSP+num_SSP;
    }
    printf("Sum = %d\n\n",sum_SSP);

    return 0;
}
/*
No Number Given For addition !!! Exitting Now...

Usage  : CommandLineArgumentsApplication <first Number> <second number>...
*/