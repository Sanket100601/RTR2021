#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(void)
{
    // variable declaration
    char ch_SSP, ch_i_SSP;
    unsigned int ascii_ch_SSP = 0;

    // code 
    printf("\n\n");
    printf("Enter the First Character Of First Name : ");
    ch_SSP=getch();

    ch_SSP=toupper(ch_SSP);

    for(ch_i_SSP='A'; ch_i_SSP <='Z'; ch_i_SSP++)
    {
        if(ch_SSP == ch_i_SSP)
        {
            ascii_ch_SSP = (unsigned int)ch_SSP;
            goto result_output; //program flow jumps directlyy to label "result_output"
        }
    }

    printf("\n\n");
    printf("Goto statement not executed so printing \"Hello World!!!\".\n");    //will be omitted if 'goto' statement is executed

    result_output:  // Label itself does not alter flow of program. Following code is executed regardless of wheteher goto statement is exwcuted or not.
        printf("\n\n");

        if(ascii_ch_SSP == 0)
        {
            printf("You Must Have A Strange Name! Could Not Finnd The The  Character '%c' In The Entire English Alphabet!\n",ch_SSP);
        }
        else
        {
            printf("Character '%c' found. It Has ASCII value %u.\n",ch_SSP,ascii_ch_SSP);
        }
        printf("\n\n");
        
        return 0;
}
