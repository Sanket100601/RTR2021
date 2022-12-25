#include<stdio.h>
int main(void)
{
    //variable declaration 
    float f_SSP;
    float f_num_SSP=1.7f;    //simply Change Value to9 get outputs 

    //code 
    printf("\n\n");

    printf("Printing numbers from %f to %f : \n\n",f_num_SSP,(f_num_SSP*10.0f));
    
    for(f_SSP=f_num_SSP;f_SSP<=(f_num_SSP*10.0f);f_SSP=f_SSP+f_num_SSP)
    {
        printf("\t%f\n",f_SSP);
    }

    printf("\n\n");

    return 0;
}
/*
Printing numbers from 1.700000 to 17.000000 : 

        1.700000
        3.400000
        5.100000
        6.800000
        8.500000
        10.200000
        11.900000
        13.599999
        15.299999
        17.000000
*/