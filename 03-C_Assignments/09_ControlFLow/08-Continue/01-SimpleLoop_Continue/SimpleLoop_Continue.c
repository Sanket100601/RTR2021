#include<stdio.h>
int main(void)
{
    //variables declaration
    int i_SSP;

    printf("\n\n");

    printf("Printing Event Numbers From 0 to 100: \n\n");

    for(i_SSP=0;i_SSP<=100;i_SSP++)
    {
        /* code */
        //condition for number to be even => Division of a number by 2 leaves remainder 0
        //if remainder is not 0 it is a doo number...
        if (i_SSP%2 != 0)
        {
            /* code */
            continue;
        }
        else
        {
            printf("\t%d\n",i_SSP);
        }
    }
    printf("\n\n");

    return 0;
}
/*
Printing Event Numbers From 0 to 100: 

        0
        2
        4
        6
        8
        10
        12
        14
        16
        18
        20
        22
        24
        26
        28
        30
        32
        34
        36
        38
        40
        42
        44
        46
        48
        50
        52
        54
        56
        58
        60
        62
        64
        66
        68
        70
        72
        74
        76
        78
        80
        82
        84
        86
        88
        90
        92
        94
        96
        98
        100
*/
