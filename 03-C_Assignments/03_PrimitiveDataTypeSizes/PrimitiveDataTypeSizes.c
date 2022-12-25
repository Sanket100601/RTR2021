//RTR2021-062-Sanket_Pawar
#include<stdio.h>

int main(void)
{
    //code
    printf("\n\n");
    
    printf("size of 'int'                   =%ld bytes\n", sizeof(int));
    printf("size of 'unsigned int'          =%ld bytes\n", sizeof(unsigned int));
    printf("size of 'long'                  =%ld bytes\n", sizeof(long));
    printf("size of 'long long'             =%ld bytes\n", sizeof(long long));

    printf("size of 'float'                 =%ld bytes\n", sizeof(float));
    printf("size of 'double'                =%ld bytes\n", sizeof(double));
    printf("size of 'long double'           =%ld bytes\n", sizeof(long double));
    
    printf("\n\n");
    return 0;
}
/*
size of 'int'                   =4 bytes
size of 'unsigned int'          =4 bytes
size of 'long'                  =4 bytes
size of 'long long'             =8 bytes
size of 'float'                 =4 bytes
size of 'double'                =8 bytes
size of 'long double'           =12 bytes
*/
