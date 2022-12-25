#include <stdio.h>

#define MAX_NAME_LENGTH 100

struct Employee
{
    char name_SSP[MAX_NAME_LENGTH];
    unsigned int age_SSP;
    char gender_SSP;
    double salary_SSP;
};

struct MyData
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
    char c_SSP;
};

int main(void)
{
    // Typedefs
    typedef struct Employee MY_EMPLOYEE_TYPE;
    typedef struct MyData MY_DATA_TYPE;
 
    // variable declarations
    struct Employee emp_SSP = {"Funny", 25, 'M', 10000.00};
    MY_EMPLOYEE_TYPE emp_typedef_SSP = {"Bunny", 23, 'F', 20400.00};
 
    struct MyData md_SSP = {30, 11.45f, 26.122017, 'X'};
    MY_DATA_TYPE md_typedef_SSP;
 
    // code
    md_typedef_SSP.i_SSP = 9;
    md_typedef_SSP.f_SSP = 1.5f;
    md_typedef_SSP.d_SSP = 8.041997;
    md_typedef_SSP.c_SSP = 'P';
    
    printf("\n\n");
    printf("struct Employee : \n\n");
    printf("emp.name = %s\n", emp_SSP.name_SSP);
    printf("emp.age = %d\n", emp_SSP.age_SSP);
    printf("emp.gender = %c\n", emp_SSP.gender_SSP);
    printf("emp.salary = %lf\n", emp_SSP.salary_SSP);
    
    printf("\n\n");
    printf("MY_EMPLOYEE_TYPE : \n\n");
    printf("emp_typedef.name = %s\n", emp_typedef_SSP.name_SSP);
    printf("emp_typedef.age = %d\n", emp_typedef_SSP.age_SSP);
    printf("emp_typedef.gender = %c\n", emp_typedef_SSP.gender_SSP);
    printf("emp_typedef.salary = %lf\n", emp_typedef_SSP.salary_SSP);
    
    printf("\n\n");
    printf("struct MyData : \n\n");
    printf("md.i = %d\n", md_SSP.i_SSP);
    printf("md.f = %f\n", md_SSP.f_SSP);
    printf("md.d = %lf\n", md_SSP.d_SSP);
    printf("md.c = %c\n", md_SSP.c_SSP);
    
    printf("\n\n");
    printf("MY_DATA_TYPE : \n\n");
    printf("md_typedef.i = %d\n", md_typedef_SSP.i_SSP);
    printf("md_typedef.f = %f\n", md_typedef_SSP.f_SSP);
    printf("md_typedef.d = %lf\n", md_typedef_SSP.d_SSP);
    printf("md_typedef.c = %c\n", md_typedef_SSP.c_SSP);
    
    printf("\n\n");
    
    return(0);
}