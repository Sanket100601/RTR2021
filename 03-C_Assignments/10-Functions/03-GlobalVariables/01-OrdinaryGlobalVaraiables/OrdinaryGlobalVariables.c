#include<stdio.h>

//Global Scope

int global_count_SSP=0;

int main(void)
{
    //function prototypes
    void change_count_one(void);
    void change_count_two(void);
    void change_count_three(void);

    //code
    printf("\n");

    printf("main() : Valueof global_count = %d\n",global_count_SSP);

    change_count_one();
    change_count_two();
    change_count_three();

    printf("\n");
    return 0;
}

//Global Scope

void change_count_one(void)
{
    //code
    global_count_SSP=100;
    printf("change_count_one() : Value of global_count = %d\n",global_count_SSP);
}

//global scope
void change_count_two(void)
{
    //code
    global_count_SSP=global_count_SSP+1;
    printf("change_count_two() : Value of global_count = %d\n",global_count_SSP);
}

//global scope
void change_count_three(void)
{
    //code
    global_count_SSP=global_count_SSP+10;
    printf("change_count_three() : Value of global_count = %d\n",global_count_SSP);
}

/*
main() : Valueof global_count = 0
change_count_one() : Value of global_count = 100
change_count_two() : Value of global_count = 101
change_count_three() : Value of global_count = 111
*/
