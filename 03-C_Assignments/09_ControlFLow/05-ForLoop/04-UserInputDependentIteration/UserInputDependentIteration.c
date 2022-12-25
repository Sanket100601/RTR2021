#include<stdio.h>
int main(void)
{
    //variables declarations 
    int i_num_SSP,num_SSP,i_SSP;

    //code
    printf("\n\n");

    printf("Enter an iteration value from which iteration must begin: ");
    scanf("%d",&i_num_SSP);

    printf("How many digits you want to print from %d onwards ? : ",i_num_SSP);
    scanf("%d",&num_SSP);

    printf("printing digits %d to %d : \n\n",i_num_SSP,(i_num_SSP+num_SSP));

    for(i_SSP=i_num_SSP;i_SSP<=(i_num_SSP+num_SSP);i_SSP++)
    {
        printf("\t%d\n",i_SSP);
    }

    printf("\n\n");
}
/*
Enter an iteration value from which iteration must begin: 1
How many digits you want to print from 1 onwards ? : 10
printing digits 1 to 11 : 

        1
        2
        3
        4
        5
        6
        7
        8
        9
        10
        11
*/