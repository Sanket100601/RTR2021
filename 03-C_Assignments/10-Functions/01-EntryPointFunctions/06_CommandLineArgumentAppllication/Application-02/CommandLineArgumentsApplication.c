#include<stdio.h>   //'stdio.h' contains Declaration of 'printf()'
#include<stdlib.h>  //'stdlib.h' contains declaration of 'exit()'

int main(int argc,char*argv[],char*envp[])
{
    //variable declarion
    int i_SSP;

    //code
    if(i_SSP=!4)    //program  name + First name + Middle name + surname = 4 command line  arguments are required 
    {
        printf("\n\n");
        printf("Inavlid usage Exiting now...\n\n");
        printf("Usage Command Line Arguments Apllication <first name> <middle name> <surname>\n\n" );
        exit(0);
    }

    // this programs Prints Your full name as entered in thecommand line  arguments
    printf("\n\n");
    printf("Your Full Name is : ");
    for(i_SSP=1;i_SSP<argc;i_SSP++) //loop starts from i=1 because, i=0 will result in argv[i_SSP] = argv[0] which is the  name of program itself
    {
        printf("%s",argv[i_SSP]);
    }
    printf("\n\n");

    return 0;
}
// Your Full Name is : 