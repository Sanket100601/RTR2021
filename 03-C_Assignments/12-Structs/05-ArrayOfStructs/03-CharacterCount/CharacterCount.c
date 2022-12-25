#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX_STRING_LENGTH 1024
struct CharacterCount
{
    char ch_SSP;
    int ch_count_SSP;
} character_and_count_SSP[] = { { 'A', 0 }, //character_and_count[0].ch = 'A' & character_and_count[0].ch_count = 0
                                { 'B', 0 }, //character_and_count[1].ch = 'B' & character_and_count[1].ch_count = 0
                                {'C', 0 }, //character_and_count[2].ch = 'C' & character_and_count[2].ch_count = 0
                                { 'D', 0 }, //character_and_count[3].ch = 'D' & character_and_count[3].ch_count = 0
                                { 'E', 0 }, //character_and_count[4].ch = 'E' & character_and_count[4].ch_count = 0
                                { 'F', 0 },
                                { 'G', 0 },
                                { 'H', 0 },
                                { 'I', 0 },
                                { 'J', 0 },
                                { 'K', 0 },
                                { 'L', 0 },
                                { 'M', 0 },
                                { 'N', 0 },
                                { 'O', 0 },
                                { 'P', 0 },
                                { 'Q', 0 },
                                { 'R', 0 },
                                { 'S', 0 },
                                { 'T', 0 },
                                { 'U', 0 },
                                { 'V', 0 },
                                { 'W', 0 },
                                { 'X', 0 },
                                { 'Y', 0 },
                                { 'Z', 0 } }; //character_and_count[25].ch = 'Z' & character_and_count[25].ch_count = 0

#define SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS sizeof(character_and_count_SSP)
#define SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS sizeof(character_and_count_SSP[0])
#define NUM_ELEMENTS_IN_ARRAY (SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS / SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS)

// ENTRY POINT FUNCTION
int main(void)
{
    //variable declarations
    char str_SSP[MAX_STRING_LENGTH];
    int i_SSP, j_SSP, actual_string_length_SSP = 0;
   
    //code
    printf("\n\n");
    printf("Enter A String : \n\n");
    fgets(str_SSP, MAX_STRING_LENGTH,stdin);
    actual_string_length_SSP = strlen(str_SSP);
    printf("\n\n");
    printf("The String You Have Entered Is : \n\n");
    printf("%s\n\n", str_SSP);
    for (i_SSP = 0; i_SSP < actual_string_length_SSP; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < NUM_ELEMENTS_IN_ARRAY; j_SSP++) //Run every character of the input string through the entire alphabet (A TO Z)
        {
            str_SSP[i_SSP] = toupper(str_SSP[i_SSP]); //If input character is in lower case, turn it to upper case for comparison
                if (str_SSP[i_SSP] == character_and_count_SSP[j_SSP].ch_SSP) //If character is present...
                    character_and_count_SSP[j_SSP].ch_count_SSP++; //Increment its count by 1 ...
        }
    }
    printf("\n\n");
    printf("The Number Of Occurences Of ALL Characters From The Alphabet Are As Follows : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ELEMENTS_IN_ARRAY; i_SSP++)
    {
        printf("Character %c = %d\n", character_and_count_SSP[i_SSP].ch_SSP, character_and_count_SSP[i_SSP].ch_count_SSP);
    }
    printf("\n\n");
    
    return(0);
}