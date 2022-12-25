#include<stdio.h>
int main(void)
{
    //code
    printf("\n\n");
    printf("**********************************************************************************************************");
    printf("\n\n");

    printf("hello world !!!\n\n");
    
    int a_SSP=13;
    printf("Integer decimal value of 'a' =%d\n",a_SSP);
    printf("Integer octal value of 'a' =%o\n",a_SSP);
    printf("Integer Hexadecimal value of 'a' (Hexadecimal Letters in lower case)=%x\n",a_SSP);
    printf("Integer Hexadicimal value of 'a' (Hexadecimal Letter in Upper case) =%X\n",a_SSP);

    char ch_SSP='A';
    printf("Character ch_SSP=%c\n",ch_SSP);
    char str_SSP[]="AstroMediComp's Real Time Rendering batch";
    printf("string str_SSP=%s\n\n",str_SSP);

    long num_SSP=30121995L;
    printf("Long Integer=%ld\n\n",num_SSP);

    unsigned int b_SSP=7;
    printf("Unsigned Integer 'b' %u\n\n",b_SSP);

    float f_num_SSP=3012.1995;
    printf("Floatinng point number with just %%f 'f_num_SSP'=%f\n",f_num_SSP);
    printf("Floatinng point number with %%4.2f 'f_num_SSP'=%4.2f\n",f_num_SSP);
    printf("Floatinng point number with %%6.5f 'f_num_SSP'=%6.5f\n",f_num_SSP);

    double d_pi_SSP = 3.14159265358979323846;
	printf("Double Precision Floating Point Number Without Exponential = %g\n", d_pi_SSP);
	printf("Double Precision Floating Point Number With Exponential (Lower Case) = %e\n", d_pi_SSP);
	printf("Double Precision Floating Point Number With Exponential (Upper Case) = %E\n\n", d_pi_SSP);
	printf("Double Hexadecimal Value Of 'd_pi_SSP' (Hexadecimal Letters In Lower Case) = %a\n", d_pi_SSP);
	printf("Double Hexadecimal Value Of 'd_pi-SSP' (Hexadecimal Letters In Upper Case) = %A\n\n", d_pi_SSP);
    
    printf("**************************************************************************************\n");
    printf("\n\n");
	return 0;
}
/*
**********************************************************************************************************

hello world !!!

Integer decimal value of 'a' =13
Integer octal value of 'a' =15
Integer Hexadecimal value of 'a' (Hexadecimal Letters in lower case)=d
Integer Hexadicimal value of 'a' (Hexadecimal Letter in Upper case) =D
Character ch_SSP=A
string str_SSP=AstroMediComp's Real Time Rendering batch

Long Integer=30121995

Unsigned Integer 'b' 7

Floatinng point number with just %f 'f_num_SSP'=3012.199463
Floatinng point number with %4.2f 'f_num_SSP'=3012.20
Floatinng point number with %6.5f 'f_num_SSP'=3012.19946
Double Precision Floating Point Number Without Exponential = 3.14159
Double Precision Floating Point Number With Exponential (Lower Case) = 3.141593e+000
Double Precision Floating Point Number With Exponential (Upper Case) = 3.141593E+000

Double Hexadecimal Value Of 'd_pi_SSP' (Hexadecimal Letters In Lower Case) = 0x1.921fb5p+1
Double Hexadecimal Value Of 'd_pi-SSP' (Hexadecimal Letters In Upper Case) = 0X1.921FB5P+1

**************************************************************************************
*/