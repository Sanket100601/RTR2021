#include<stdio.h>
int main(void)
{
    //variable Declarations 
    int i_SSP,j_SSP;

    //code
    printf("\n\n");
    
    printf("Outer Loops prints Odd number between 1 to 10. \n\n");
    printf("Inner Loop prints Even Number Between 1 to 10 For Every Odd Number Printed by Outer Loop. \n\n");

    //condition for number to be even => Division of a number by 2 leaves remainder 0
    //condition for number to be odd => Division of a number by 2 leaves remainder 1

    for (i_SSP = 1; i_SSP <= 10; i_SSP++)
    {
        if (i_SSP%2!=0) //If Number is Odd
        {
            printf("i_SSP=%d\n",i_SSP);
            printf("-------------------\n");
            for (j_SSP = 0; j_SSP <= 10; j_SSP++)
            {
                if (j_SSP%2==0)     //If Number is Even
                {
                    printf("\tj_SSP=%d\n",j_SSP);
                }
                else    //If Number Is Odd
                {
                    continue;
                }
            }
            printf("\n\n");
        }
        else
        {
            continue;
        }
    }
    return 0;
}
/*
Outer Loops prints Odd number between 1 to 10. 

Inner Loop prints Even Number Between 1 to 10 For Every Odd Number Printed by Outer Loop.

i_SSP=1
-------------------
        j_SSP=0
        j_SSP=2
        j_SSP=4
        j_SSP=6
        j_SSP=8
        j_SSP=10


i_SSP=3
-------------------
        j_SSP=0
        j_SSP=2
        j_SSP=4
        j_SSP=6
        j_SSP=8
        j_SSP=10


i_SSP=5
-------------------
        j_SSP=0
        j_SSP=2
        j_SSP=4
        j_SSP=6
        j_SSP=8
        j_SSP=10


i_SSP=7
-------------------
        j_SSP=0
        j_SSP=2
        j_SSP=4
        j_SSP=6
        j_SSP=8
        j_SSP=10


i_SSP=9
-------------------
        j_SSP=0
        j_SSP=2
        j_SSP=4
        j_SSP=6
        j_SSP=8
        j_SSP=10
*/