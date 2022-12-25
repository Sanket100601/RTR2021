#include<stdio.h>

// Global typedef
typedef int MY_INT;     // "type int has been redefined as MY_INT...noe MY_INT can be treated as int"

int main(void)
{
    // Function Prototype
    MY_INT Add(MY_INT,MY_INT);

    // Typedefs
    typedef int MY_INT; 
    typedef float SSP_FLOAT;
    typedef char CHARACTER;
    typedef double MY_DOUBLE;

    // ***** Just Like Win32SDK !!! *****
    typedef unsigned int UINT;
    typedef UINT HANDLE;
    typedef HANDLE HWND;
    typedef HWND HINSTANCE;

    // Variable Declaration
    MY_INT a_SSP = 10,i_SSP;
    MY_INT iArray_SSP[]={9,18,27,36,45,54,63,72,81,90};

    SSP_FLOAT f_SSP=30.9f;
    const SSP_FLOAT f_SSP_pi=3.14f;
    CHARACTER ch_SSP='*';
    CHARACTER chArray_01_SSP[]="Hello";
    CHARACTER chArray_02_SSP[][10]={"RTR","BATCH","2020-2021"};

    MY_DOUBLE d_SSP=8.041997;

    UINT uint=3456;
    HANDLE handle=987;
    HWND hwnd=9876;
    HINSTANCE hInstance=14466;

    // code
    printf("\n\n");
    printf("Type MY_INT variable a = %d\n", a_SSP);
    printf("\n\n");

    for (i_SSP = 0; i_SSP < (sizeof(iArray_SSP) / sizeof(int)); i_SSP++)
    {
        printf("Type MY_INT array variable iArray[%d] = %d\n", i_SSP, iArray_SSP[i_SSP]);   
    }
    printf("\n\n");
    
    printf("\n\n");
    printf("Type SSP_FLOAT variable f = %f\n", f_SSP);
    printf("Type SSP_FLOAT constanct f_ssp_pi = %f\n", f_SSP_pi);

    printf("\n\n");
    printf("Type MY_DOUBLE variable d = %lf\n", d_SSP);
    printf("\n\n");
    printf("Type CHARACTER variable ch = %c\n", ch_SSP);
    printf("\n\n");
    printf("Type CHARACTER array variable chArray_01 = %s\n", chArray_01_SSP);
    printf("\n\n");
    for (i_SSP = 0; i_SSP < (sizeof(chArray_02_SSP) / sizeof(chArray_02_SSP[0])); i_SSP++)
    {
        printf("%s\t", chArray_02_SSP[i_SSP]);
    }
    printf("\n\n");
    printf("\n\n");
    printf("Type UINT variable uint = %u\n\n", uint);
    printf("Type HANDLE variable handle = %u\n\n", handle);
    printf("Type HWND variable hwnd = %u\n\n", hwnd);
    printf("Type HINSTANCE variable hInstance = %u\n\n", hInstance);
    printf("\n\n");
    MY_INT x_SSP = 90;
    MY_INT y_SSP = 30;
    MY_INT ret_SSP;
 
    ret_SSP = Add(x_SSP, y_SSP);
    printf("ret = %d\n\n", ret_SSP);
    return(0);
}

MY_INT Add(MY_INT a_SSP, MY_INT b_SSP)
{
     // code
    MY_INT c_SSP;
    c_SSP = a_SSP + b_SSP;
    return(c_SSP);
}