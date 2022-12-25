#include<stdio.h>
int main(void)
{
    //variables declaration
    char option_SSP,ch_SSP;
    
    //code
    printf("\n\n");
    printf("Once the infinite Loop Begins, Enter 'Q' or 'q' to Quite the Infinite loop: \n\n");
    printf("Enter 'Y' or 'y' to initiate user controlled Infinite Loop: ");
    printf("\n\n");
    option_SSP=getch();
    if(option_SSP=='Y' || option_SSP=='y')
    {
        while (1)   //infinite loop
        {
            /* code */
            printf("In Loop...\n");
            ch_SSP=getch();
            if(ch_SSP=='Q' || ch_SSP=='q')
            break;  //User Controlled Exiting from Infinite Loop 
        }
    }
    printf("\n\n");
    printf("Exiting user Controlled Infinite Loop...");
    printf("\n\n");

    return 0;
}
  /*
  Once the infinite Loop Begins, Enter 'Q' or 'q' to Quite the Infinite loop: 

Enter 'Y' or 'y' to initiate user controlled Infinite Loop:

In Loop...


Exiting user Controlled Infinite Loop...
*/      
        