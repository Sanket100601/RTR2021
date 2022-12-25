#include <stdio.h>
#include <string.h>
#define NAME_LENGTH 100
#define MARITAL_STATUS 10
struct Employee
{
    char name_SSP[NAME_LENGTH];
    int age_SSP;
    float salary_SSP;
    char sex_SSP;
    char marital_status_SSP[MARITAL_STATUS];
};
int main(void)
{
    //variable delarations
    struct Employee EmployeeRecord[5]; //An array of 5 structs - Each being type 'struct Employee'
    char employee_rajesh[] = "Rajesh";
    char employee_sameer[] = "Sameer";
    char employee_kalyani[] = "Kalyani";
    char employee_sonali[] = "Sonali";
    char employee_shantanu[] = "Shantanu";
    int i_SSP;
    //code
    // ****** HARD-CODED INITIALIZATION OF ARRAY OF 'struct Employee' ******
    // ****** EMPLOYEE 1 ******
    strcpy(EmployeeRecord[0].name_SSP, employee_rajesh);
    EmployeeRecord[0].age_SSP = 30;
    EmployeeRecord[0].sex_SSP = 'M';
    EmployeeRecord[0].salary_SSP = 50000.0f;
    strcpy(EmployeeRecord[0].marital_status_SSP, "Unmarried");
    // ****** EMPLOYEE 2 ******
    strcpy(EmployeeRecord[1].name_SSP, employee_sameer);
    EmployeeRecord[1].age_SSP = 32;
    EmployeeRecord[1].sex_SSP = 'M';    
    EmployeeRecord[1].salary_SSP = 60000.0f;
    strcpy(EmployeeRecord[1].marital_status_SSP, "Married");
    // ****** EMPLOYEE 3 ******
    strcpy(EmployeeRecord[2].name_SSP, employee_kalyani);
    EmployeeRecord[2].age_SSP = 29;
    EmployeeRecord[2].sex_SSP = 'F';
    EmployeeRecord[2].salary_SSP = 62000.0f;
    strcpy(EmployeeRecord[2].marital_status_SSP, "Unmarried");
    // ****** EMPLOYEE 4 ******
    strcpy(EmployeeRecord[3].name_SSP, employee_sonali);
    EmployeeRecord[3].age_SSP = 33;
    EmployeeRecord[3].sex_SSP = 'F';
    EmployeeRecord[3].salary_SSP = 50000.0f;
    strcpy(EmployeeRecord[3].marital_status_SSP, "Married");
    // ****** EMPLOYEE 5 ******
    strcpy(EmployeeRecord[4].name_SSP, employee_shantanu);
    EmployeeRecord[4].age_SSP = 35;
    EmployeeRecord[4].sex_SSP = 'M';
    EmployeeRecord[4].salary_SSP = 55000.0f;
    strcpy(EmployeeRecord[4].marital_status_SSP, "Married");
    // *** DISPLAY ***
    printf("\n\n");
    printf("****** DISPLAYING EMPLOYEE RECORDS ******\n\n");
    for (i_SSP = 0; i_SSP < 5; i_SSP++)
    {
        printf("****** EMPLOYEE NUMBER %d ******\n\n", (i_SSP + 1));
        printf("Name : %s\n", EmployeeRecord[i_SSP].name_SSP);
        printf("Age : %d years\n", EmployeeRecord[i_SSP].age_SSP);
        
        if (EmployeeRecord[i_SSP].sex_SSP == 'M' || EmployeeRecord[i_SSP].sex_SSP == 'm')
            printf("Sex : Male\n");
        else
            printf("Sex : Female\n");
        
        printf("Salary : Rs. %f\n", EmployeeRecord[i_SSP].salary_SSP);
        printf("Marital Status : %s\n", EmployeeRecord[i_SSP].marital_status_SSP);
        printf("\n\n");
    }
    return(0);
}