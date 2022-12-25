#include<stdio.h>
//global scope
int main(void)
{
    //variable declaration
    int a_SSP=5;

    //function prototype
    void change_count(void);

    //code
    printf("\n");
    printf("A = %d\n\n",a_SSP);

    change_count();

    change_count();

    change_count();

    return 0;
}

//global scope
void change_count(void)
{
    static int local_count_SSP=0;

    //code
    local_count_SSP=local_count_SSP+1;
    printf("local_count =%d\n",local_count_SSP);
}
/*
A = 5

local_count =1
local_count =2
local_count =3
*/