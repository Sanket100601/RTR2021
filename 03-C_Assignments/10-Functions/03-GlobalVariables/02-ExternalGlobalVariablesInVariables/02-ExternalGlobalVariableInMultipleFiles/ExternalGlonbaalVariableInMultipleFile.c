#include<stdio.h>

//global scope
int global_count_SSP=0;

int main(void)
{
    //function prototypes
    void change_count(void);
    void change_count_one(void);    //function defines in file_01.c
    void change_count_two(void);    //function defines in file_02.c

    return 0;
}

void change_count(void)
{
    //code
    global_count_SSP=global_count_SSP+1;
    printf("Global Count = %d\n",global_count_SSP);
}


