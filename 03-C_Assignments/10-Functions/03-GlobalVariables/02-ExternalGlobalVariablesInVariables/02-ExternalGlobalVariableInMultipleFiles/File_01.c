#include<stdio.h>

extern int global_count_SSP(void);
void change_count_one(void)
{
    //code
    global_count_SSP=global_count_SSP+1;
    
    printf("change_count_one() : value of global_count in File_01 = %d\n",global_count_SSP);
}