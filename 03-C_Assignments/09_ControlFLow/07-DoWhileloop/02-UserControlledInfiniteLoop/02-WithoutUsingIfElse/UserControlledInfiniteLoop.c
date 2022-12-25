#include<stdio.h>
int main(void)
{
    //variable declaration
    char option_SSP, ch_SSP='\0';

    //code
    printf("\n\n");
    printf("Once the infinite Loop Begins, Enter 'Q' or 'q' to Quite the Infinite loop: \n\n");

    do
    {
        do
        {
            printf("\n\n");
            printf("In Loop...\n");
            ch_SSP=getch(); //Control Flow Waits for character Input
        }while (ch_SSP!='Q' &&ch_SSP!='q');
        printf("\n\n");
        printf("Exiting user Controlled Infinite Loop...");

        printf("\n\n");
        printf("Do you want to begin User Controlled infinite Loop Again?...(Y/y - YES, Any Other Key - NO): ");
        option_SSP=getch(); 
    }while(option_SSP=='Y' || option_SSP=='y');
    return 0;
}
