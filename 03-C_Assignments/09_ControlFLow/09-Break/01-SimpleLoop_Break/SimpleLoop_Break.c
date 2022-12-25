#include<stdio.h>
#include<conio.h>
int main(void)
{
    //variable declarations
    int i_SSP;
    char ch_SSP;

    //code
    printf("\n\n");

    printf("Printing Even Numbers From 1 to 100 For Every user Input. Exiting the Loop When  User Enters Character 'Q' or 'q' : \n\n");
    printf("Enter the Character 'Q' or 'q' to exit Loop : \n\n");

    for ( i_SSP = 1; i_SSP <= 100; i_SSP++)
    {
        /* code */
        printf("\t%d\n",i_SSP);
        ch_SSP=getch();
        if (ch_SSP=='Q' || ch_SSP=='q')
        {
            /* code */
            break;
        }
    }
    printf("\n\n");
    printf("EXITING LOOP");
    printf("\n\n");

    return 0;
}
/*
Printing Even Numbers From 1 to 100 For Every user Input. Exiting the Loop When  User Enters Character 'Q' or 'q' : 

Enter the Character 'Q' or 'q' to exit Loop :

        1


EXITING LOOP
*/