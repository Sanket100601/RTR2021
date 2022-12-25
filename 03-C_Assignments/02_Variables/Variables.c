#include<stdio.h>

int main(void)
{
    //variables declaration
    int i_SSP =5;
    float f_SSP=3.9f;
    double d_SSP=8.041997;
    char c_SSP='A';

    //code 
    printf("\n\n");

    printf("i_SSP=%d\n",i_SSP);
    printf("f_SSP=%d\n",f_SSP);
    printf("d_SSP=%d\n",d_SSP);
    printf("c_SSP=%d\n",c_SSP);

    printf("\n\n");

    i_SSP =43;
    f_SSP=6.54;
    d_SSP=26.1294;
    c_SSP='S';

    printf("i_SSP=%d\n",i_SSP);
    printf("f_SSP=%d\n",f_SSP);
    printf("d_SSP=%d\n",d_SSP);
    printf("c_SSP=%d\n",c_SSP);

    printf("\n\n");

    return 0;
}

/*
i_SSP=5
f_SSP=1073741824
d_SSP=-1585770645
c_SSP=65


i_SSP=43
f_SSP=-1073741824
d_SSP=1539316279
c_SSP=83
*/