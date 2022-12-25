#include<stdio.h>
int main(int argc,char*argv[],char*envp[])
{
    //variable Declarations
    unsigned int num_SSP;

    //functionn prototype
    void recursive(unsigned int);

    //code
    printf("\n\n");
    printf("Enter Any Number : \n\n");
    scanf("%u",&num_SSP);

    printf("\n\n");
    printf("Output Of Recursive Function : \n\n");

    recursive(num_SSP);
    return 0;
}

void recursive(unsigned int n_SSP)
{
    //code
    printf("n = %d",n_SSP);

    if(n_SSP>0)
    {
        recursive(n_SSP-1);
    }
}
/*
Enter Any Number : 

20


Output Of Recursive Function :

n = 20n = 19n = 18n = 17n = 16n = 15n = 14n = 13n = 12n = 11n = 10n = 9n = 8n = 7n = 6n = 5n = 4n = 3n = 2n = 1n = 0
*/
