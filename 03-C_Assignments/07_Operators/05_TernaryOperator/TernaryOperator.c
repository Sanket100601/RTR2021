#include<stdio.h>

int main(void)
{
    //variable declaration
    int a_SSP,b_SSP;
    int p_SSP,q_SSP;
    char ch_result_01_SSP,ch_result_02_SSP;
    int i_result_01_SSP,i_result_02_SSP;

    //code
    printf("\n\n");

    a_SSP=7;
    b_SSP=5;
    ch_result_01_SSP = (a_SSP > b_SSP) ? 'A' : 'B';
    i_result_01_SSP = (a_SSP > b_SSP) ? a_SSP : b_SSP;
    printf("Ternary Operator Answer 1----%c and %d.\n\n",ch_result_01_SSP,i_result_01_SSP);

    p_SSP=30;
    q_SSP=50;
    ch_result_02_SSP = (p_SSP != q_SSP) ? 'P' : 'Q';
    i_result_02_SSP = (p_SSP != q_SSP) ? p_SSP : q_SSP;
    printf("Ternary Operator Answer 2----%c and %d.\n\n",ch_result_02_SSP,i_result_02_SSP);

    printf("\n\n");
    return 0;
}
