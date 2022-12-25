#include<stdio.h>

int main(void)
{
    int a_SSP=5;
    int b_SSP=10;

    //code
    printf("\n\n");
    printf("A_SSP=%d\n",a_SSP);
    printf("A_SSP=%d\n",a_SSP++);
    printf("A_SSP=%d\n",a_SSP);
    printf("A_SSP=%d\n",++a_SSP);

    printf("B_SSP=%d\n",b_SSP);
    printf("B_SSP=%d\n",b_SSP--);
    printf("B_SSP=%d\n",b_SSP);
    printf("B_SSP=%d\n",--b_SSP);

    return 0;
}
/*
A_SSP=5
A_SSP=5
A_SSP=6
A_SSP=7
B_SSP=10
B_SSP=10
B_SSP=9
B_SSP=8
*/