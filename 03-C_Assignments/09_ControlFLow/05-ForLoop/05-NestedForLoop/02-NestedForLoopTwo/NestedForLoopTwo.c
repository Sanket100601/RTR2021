#include<stdio.h>
int main(void)
{
    //variable declaration
    int i_SSP,j_SSP,k_SSP;

    //code
    printf("\n\n");
    for(i_SSP=1;i_SSP<=10;i_SSP++)
    {
        printf("i=%d\n",i_SSP);
        printf("-----------\n\n");
        for(j_SSP=1;j_SSP<=5;j_SSP++)
        {
            printf("\tj_SSP=%d\n",j_SSP);
            printf("\t---------\n\n");
            for (k_SSP = 1; k_SSP <= 3; k_SSP++)
            {
                printf("\t\tk_SSP=%d\n",k_SSP);
            }
            
        }
        printf("\n\n");
    }
    return 0;
}
/*
i=1
-----------

        j_SSP=1
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=2
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=3
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=4
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=5
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3


i=2
-----------

        j_SSP=1
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=2
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=3
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=4
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=5
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3


i=3
-----------

        j_SSP=1
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=2
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=3
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=4
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=5
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3


i=4
-----------

        j_SSP=1
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=2
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=3
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=4
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=5
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3


i=5
-----------

        j_SSP=1
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=2
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=3
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=4
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=5
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3


i=6
-----------

        j_SSP=1
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=2
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=3
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=4
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=5
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3


i=7
-----------

        j_SSP=1
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=2
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=3
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=4
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=5
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3


i=8
-----------

        j_SSP=1
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=2
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=3
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=4
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=5
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3


i=9
-----------

        j_SSP=1
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=2
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=3
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=4
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=5
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3


i=10
-----------

        j_SSP=1
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=2
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=3
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=4
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
        j_SSP=5
        ---------

                k_SSP=1
                k_SSP=2
                k_SSP=3
*/
