#include <stdio.h>
int main(void)
{
    //variable declaraions
    int iArrayOne_SSP[10];
    int iArrayTwo_SSP[10];
    //code
    // ****** iArrayOne[] ******
    iArrayOne_SSP[0] = 3;
    iArrayOne_SSP[1] = 6;
    iArrayOne_SSP[2] = 9;
    iArrayOne_SSP[3] = 12;
    iArrayOne_SSP[4] = 15;
    iArrayOne_SSP[5] = 18;
    iArrayOne_SSP[6] = 21;  
    iArrayOne_SSP[7] = 24;
    iArrayOne_SSP[8] = 27;
    iArrayOne_SSP[9] = 30;
    printf("\n\n");
    printf("Piece-meal (Hard-coded) Assignment And Display Of Elements to Array 'iArrayOne[]': \n\n");
    printf("1st Element Of Array 'iArrayOne[]' Or Element At 0th Index Of Array 'iArrayOne[]' = %d\n", iArrayOne_SSP[0]);
    printf("2nd Element Of Array 'iArrayOne[]' Or Element At 1st Index Of Array 'iArrayOne[]' = %d\n", iArrayOne_SSP[1]);
    printf("3rd Element Of Array 'iArrayOne[]' Or Element At 2nd Index Of Array 'iArrayOne[]' = %d\n", iArrayOne_SSP[2]);
    printf("4th Element Of Array 'iArrayOne[]' Or Element At 3rd Index Of Array 'iArrayOne[]' = %d\n", iArrayOne_SSP[3]);
    printf("5th Element Of Array 'iArrayOne[]' Or Element At 4th Index Of Array 'iArrayOne[]' = %d\n", iArrayOne_SSP[4]);
    printf("6th Element Of Array 'iArrayOne[]' Or Element At 5th Index Of Array 'iArrayOne[]' = %d\n", iArrayOne_SSP[5]);
    printf("7th Element Of Array 'iArrayOne[]' Or Element At 6th Index Of Array 'iArrayOne[]' = %d\n", iArrayOne_SSP[6]);
    printf("8th Element Of Array 'iArrayOne[]' Or Element At 7th Index Of Array 'iArrayOne[]' = %d\n", iArrayOne_SSP[7]);
    printf("9th Element Of Array 'iArrayOne[]' Or Element At 8th Index Of Array 'iArrayOne[]' = %d\n", iArrayOne_SSP[8]);
    printf("10th Element Of Array 'iArrayOne[]' Or Element At 9th Index Of Array 'iArrayOne[]' = %d\n\n", iArrayOne_SSP[9]);
    // ****** iArrayTwo[] ******
    printf("\n\n");
    printf("Enter 1st Element Of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo_SSP[0]);
    printf("Enter 2nd Element Of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo_SSP[1]);
    printf("Enter 3rd Element Of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo_SSP[2]);
    printf("Enter 4th Element Of Array 'iArrayTwo[]' : ");  
    scanf("%d", &iArrayTwo_SSP[3]);
    printf("Enter 5th Element Of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo_SSP[4]);
    printf("Enter 6th Element Of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo_SSP[5]); 
    printf("Enter 7th Element Of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo_SSP[6]);
    printf("Enter 8th Element Of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo_SSP[7]);
    printf("Enter 9th Element Of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo_SSP[8]);
    printf("Enter 10th Element Of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo_SSP[9]);
    printf("\n\n");
    printf("Piece-meal (User Input) Assignment And Display Of Elements to Array 'iArrayTwo[]' : \n\n");
    printf("1st Element Of Array 'iArrayTwo[]' Or Element At 0th Index Of Array 'iArrayTwo[]' = %d\n", iArrayTwo_SSP[0]);
    printf("2nd Element Of Array 'iArrayTwo[]' Or Element At 1st Index Of Array 'iArrayTwo[]' = %d\n", iArrayTwo_SSP[1]);   
    printf("3rd Element Of Array 'iArrayTwo[]' Or Element At 2nd Index Of Array 'iArrayTwo[]' = %d\n", iArrayTwo_SSP[2]);
    printf("4th Element Of Array 'iArrayTwo[]' Or Element At 3rd Index Of Array 'iArrayTwo[]' = %d\n", iArrayTwo_SSP[3]);
    printf("5th Element Of Array 'iArrayTwo[]' Or Element At 4th Index Of Array 'iArrayTwo[]' = %d\n", iArrayTwo_SSP[4]);
    printf("6th Element Of Array 'iArrayTwo[]' Or Element At 5th Index Of Array 'iArrayTwo[]' = %d\n", iArrayTwo_SSP[5]);
    printf("7th Element Of Array 'iArrayTwo[]' Or Element At 6th Index Of Array 'iArrayTwo[]' = %d\n", iArrayTwo_SSP[6]);
    printf("8th Element Of Array 'iArrayTwo[]' Or Element At 7th Index Of Array 'iArrayTwo[]' = %d\n", iArrayTwo_SSP[7]);
    printf("9th Element Of Array 'iArrayTwo[]' Or Element At 8th Index Of Array 'iArrayTwo[]' = %d\n", iArrayTwo_SSP[8]);
    printf("10th Element Of Array 'iArrayTwo[]' Or Element At 9th Index Of Array 'iArrayTwo[]' = %d\n\n", iArrayTwo_SSP[9]);
    return(0);
}
/*
Piece-meal (Hard-coded) Assignment And Display Of Elements to Array 'iArrayOne[]': 

1st Element Of Array 'iArrayOne[]' Or Element At 0th Index Of Array 'iArrayOne[]' = 3
2nd Element Of Array 'iArrayOne[]' Or Element At 1st Index Of Array 'iArrayOne[]' = 6
3rd Element Of Array 'iArrayOne[]' Or Element At 2nd Index Of Array 'iArrayOne[]' = 9
4th Element Of Array 'iArrayOne[]' Or Element At 3rd Index Of Array 'iArrayOne[]' = 12
5th Element Of Array 'iArrayOne[]' Or Element At 4th Index Of Array 'iArrayOne[]' = 15
6th Element Of Array 'iArrayOne[]' Or Element At 5th Index Of Array 'iArrayOne[]' = 18
7th Element Of Array 'iArrayOne[]' Or Element At 6th Index Of Array 'iArrayOne[]' = 21
8th Element Of Array 'iArrayOne[]' Or Element At 7th Index Of Array 'iArrayOne[]' = 24
9th Element Of Array 'iArrayOne[]' Or Element At 8th Index Of Array 'iArrayOne[]' = 27
10th Element Of Array 'iArrayOne[]' Or Element At 9th Index Of Array 'iArrayOne[]' = 30



Enter 1st Element Of Array 'iArrayTwo[]' : 2
Enter 2nd Element Of Array 'iArrayTwo[]' : 1
Enter 3rd Element Of Array 'iArrayTwo[]' : 32
Enter 4th Element Of Array 'iArrayTwo[]' : 42
Enter 5th Element Of Array 'iArrayTwo[]' : 4
Enter 6th Element Of Array 'iArrayTwo[]' : 3
Enter 7th Element Of Array 'iArrayTwo[]' : 24
Enter 8th Element Of Array 'iArrayTwo[]' : 5
Enter 9th Element Of Array 'iArrayTwo[]' : 
2
Enter 10th Element Of Array 'iArrayTwo[]' : 3


Piece-meal (User Input) Assignment And Display Of Elements to Array 'iArrayTwo[]' :

1st Element Of Array 'iArrayTwo[]' Or Element At 0th Index Of Array 'iArrayTwo[]' = 2
2nd Element Of Array 'iArrayTwo[]' Or Element At 1st Index Of Array 'iArrayTwo[]' = 1
3rd Element Of Array 'iArrayTwo[]' Or Element At 2nd Index Of Array 'iArrayTwo[]' = 32
4th Element Of Array 'iArrayTwo[]' Or Element At 3rd Index Of Array 'iArrayTwo[]' = 42
5th Element Of Array 'iArrayTwo[]' Or Element At 4th Index Of Array 'iArrayTwo[]' = 4
6th Element Of Array 'iArrayTwo[]' Or Element At 5th Index Of Array 'iArrayTwo[]' = 3
7th Element Of Array 'iArrayTwo[]' Or Element At 6th Index Of Array 'iArrayTwo[]' = 24
8th Element Of Array 'iArrayTwo[]' Or Element At 7th Index Of Array 'iArrayTwo[]' = 5
9th Element Of Array 'iArrayTwo[]' Or Element At 8th Index Of Array 'iArrayTwo[]' = 2
10th Element Of Array 'iArrayTwo[]' Or Element At 9th Index Of Array 'iArrayTwo[]' = 3
*/
