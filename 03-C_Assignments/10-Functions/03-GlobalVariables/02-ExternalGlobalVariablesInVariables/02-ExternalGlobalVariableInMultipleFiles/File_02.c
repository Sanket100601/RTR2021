#include<stdio.h>
void main()
{
    //code
    extern int global_count_SSP;
    global_count_SSP = global_count_SSP+1;
    printf("change_count_two() : Value of global_count in File_02 = %d\n",global_count_SSP); 
}
