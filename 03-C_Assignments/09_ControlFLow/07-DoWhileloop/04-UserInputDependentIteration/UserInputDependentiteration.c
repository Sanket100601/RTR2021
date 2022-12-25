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

    i_SSP=i_num_SSP;
    do
    {
        /* code */
        printf("\t%d\n",i_SSP);
        i_SSP++;
    } while (i_SSP<=(i_num_SSP+num_SSP));
    printf("\n\n");

    return 0;
}