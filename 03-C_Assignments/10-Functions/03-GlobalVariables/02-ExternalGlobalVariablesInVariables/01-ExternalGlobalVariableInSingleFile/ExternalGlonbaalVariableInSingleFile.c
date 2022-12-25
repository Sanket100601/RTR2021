#include<stdio.h>

//Globle Scope

int main(void)
{
    //function prototypes
    void change_count(void);

    //variable declaration
    extern int global_count_SSP;

    //code
    printf("\n");
    printf("Value of global_count before change_count() = %d\n",global_count_SSP);
    change_count();
    printf("Value Of global_count after change-count() = %d\n",global_count_SSP);
    printf("\n");
    return 0;
}
int global_count_SSP = 0;

void change_count(void)
{
    global_count_SSP=5;
    printf("value Of Global_count in change_count = %d\n",global_count_SSP);
}
/*
Value of global_count before change_count() = 0
value Of Global_count in change_count = 5
Value Of global_count after change-count() = 5
*/
