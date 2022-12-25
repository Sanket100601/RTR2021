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
    while (i_SSP<=(i_num_SSP+num_SSP))
    {
        /* code */
        printf("\t%d\n",i_SSP);
        i_SSP++;
    }
    printf("\n\n");

    return 0;
}
/*
Enter an iteration value from which iteration must begin: 1
How many digits you want to print from 1 onwards ? : 100
printing digits 1 to 101 : 

        1
        2
        3
        4
        5
        6
        7
        8
        9
        10
        11
        12
        13
        14
        15
        16
        17
        18
        19
        20
        21
        22
        23
        24
        25
        26
        27
        28
        29
        30
        31
        32
        33
        34
        35
        36
        37
        38
        39
        40
        41
        42
        43
        44
        45
        46
        47
        48
        49
        50
        51
        52
        53
        54
        55
        56
        57
        58
        59
        60
        61
        62
        63
        64
        65
        66
        67
        68
        69
        70
        71
        72
        73
        74
        75
        76
        77
        78
        79
        80
        81
        82
        83
        84
        85
        86
        87
        88
        89
        90
        91
        92
        93
        94
        95
        96
        97
        98
        99
        100
        101
*/

    