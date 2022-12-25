#include<stdio.h>   //for printf()
#include<conio.h>   //for getch
//ASCII Values for 'A' to 'Z' => 65 to 90
#define CHAR_ALPHABET_UPPER_CASE_BEGINNING 65
#define CHAR_ALPHABET_UPPER_CASE_ENDING 90

//ASCII Values for 'a' to 'z' => 97 to 122
#define CHAR_ALPHABET_LOWER_CASE_BEGINNING 97
#define CHAR_ALPHABET_LOWER_CASE_ENDING 122

//ASCII VAlues for '0' to '9' => 48 to 57
#define CHAR_DIGIT_BEGINNING 48
#define CHAR_DIGIT_ENDING 57
int main(void)
{
    //variable declarations
    char ch_SSP;
    int ch_value_SSP;

    //code
    printf("\n\n");

    printf("Enter Character: ");
    ch_SSP = getch();

    printf("\n\n");

    if((ch_SSP=='A' || ch_SSP=='a')||(ch_SSP=='E' || ch_SSP=='e')||(ch_SSP=='I' || ch_SSP=='i')||(ch_SSP=='O' || ch_SSP=='o')||(ch_SSP=='U' || ch_SSP=='u'))
    {
        printf("Character \'%c\' Entered By You Is A VOWEL CHARACTER From The English Alphabet !!!\n\n",ch_SSP);
    }
    else
    {
        ch_value_SSP=(int)ch_SSP;
            //If Character has ASCII value Between 65 nad 90 Or Between 97 and 122,It is still a letter pf alphabet value but it is a Consonant and not A VOWEL
            if ((ch_value_SSP>=CHAR_ALPHABET_UPPER_CASE_BEGINNING && ch_value_SSP<=CHAR_ALPHABET_UPPER_CASE_ENDING) || (ch_value_SSP>=CHAR_ALPHABET_LOWER_CASE_BEGINNING &&ch_value_SSP<=CHAR_ALPHABET_LOWER_CASE_ENDING))
            {
                /* code */
                printf("Character \'%c\' Entered By You Is A Consonant CHARACTER From The English Alphabet !!!\n\n",ch_SSP);
            }
            else if(ch_value_SSP >= CHAR_DIGIT_BEGINNING && ch_value_SSP <= CHAR_DIGIT_ENDING)
            {
                printf("Character \'%c\' entered by you is a DIGIT Character !!!\n\n",ch_SSP);
            }
            else
            {
                printf("Character \'%c\' entered by you is a SPEIAL Character !!!\n\n",ch_SSP);
            }
    }
    return 0;
}
/*
Enter Character: 

Character 'A' Entered By You Is A VOWEL CHARACTER From The English Alphabet !!!
*/
