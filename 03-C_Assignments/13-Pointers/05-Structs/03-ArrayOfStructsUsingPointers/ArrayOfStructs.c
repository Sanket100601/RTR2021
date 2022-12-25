#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
    char name_SSP[NAME_LENGTH];
    int age_SSP;
    char sex_SSP;
    float salary_SSP;
    char marital_status_SSP;
};

int main(void)
{
    //function prototype
    void MyGetString(char[], int);

    //variable delarations
    struct Employee *pEmployeeRecord_SSP = NULL;
    int num_employees_SSP, i_SSP;

    //code
    printf("\n\n");
    printf("Enter Number Of Employees Whose Details You Want To Record : ");
    scanf("%d", &num_employees_SSP);
    printf("\n\n");
    pEmployeeRecord_SSP = (struct Employee *)malloc(sizeof(struct Employee) * num_employees_SSP);
    if (pEmployeeRecord_SSP == NULL)
    {
        printf("FAILED TO ALLOCATED MEMORY FOR %d EMPLOYEES !!! EXITTING NOW ...\n\n", num_employees_SSP);
        exit(0);
    }
    else
        printf("SUCCESSFULLY ALLOCATED MEMORY FOR %d EMPLOYEES !!!\n\n", num_employees_SSP);

    // ****** USER INPUT INITIALIZATION OF ARRAY OF 'struct Employee' ******
    for (i_SSP = 0; i_SSP < num_employees_SSP; i_SSP++)
    {
        printf("\n\n\n");
        printf("Enter Employee's Age (in years) : ");
        scanf("%d", &pEmployeeRecord_SSP[i_SSP].age_SSP);
        
        printf("\n\n");
        printf("Enter Employee's Sex (M/m For Male, F/f For Female) : ");
        pEmployeeRecord_SSP[i_SSP].sex_SSP = getch();
        printf("%c", pEmployeeRecord_SSP[i_SSP].sex_SSP);
        pEmployeeRecord_SSP[i_SSP].sex_SSP = toupper(pEmployeeRecord_SSP[i_SSP].sex_SSP);
       
        printf("\n\n\n");
        printf("Enter Employee's Salary (in Indian Rupees) : ");
        scanf("%f", &pEmployeeRecord_SSP[i_SSP].salary_SSP);
       
        printf("\n\n");
        printf("Is The Employee Married? (Y/y For Yes, N/n For No) : ");
        pEmployeeRecord_SSP[i_SSP].marital_status_SSP = getch();
        printf("%c", pEmployeeRecord_SSP[i_SSP].marital_status_SSP);
        pEmployeeRecord_SSP[i_SSP].marital_status_SSP = toupper(pEmployeeRecord_SSP[i_SSP].marital_status_SSP);
    }
    // *** DISPLAY ***
    printf("\n\n\n\n");
    printf("********** DISPLAYING EMPLOYEE RECORDS **********\n\n");
    for (i_SSP = 0; i_SSP < num_employees_SSP; i_SSP++)
    {
        printf("*********** EMPLOYEE NUMBER %d **********\n\n", (i_SSP + 1));
        printf("Name : %s\n", pEmployeeRecord_SSP[i_SSP].name_SSP);
        printf("Age : %d years\n", pEmployeeRecord_SSP[i_SSP].age_SSP);
       
        if (pEmployeeRecord_SSP[i_SSP].sex_SSP == 'M')
         printf("Sex : Male\n");
       
        else if(pEmployeeRecord_SSP[i_SSP].sex_SSP == 'F')
            printf("Sex : Female\n");
        
        else
            printf("Sex : Invalid Data Entered\n");
        printf("Salary : Rs. %f\n", pEmployeeRecord_SSP[i_SSP].salary_SSP);
        
        if (pEmployeeRecord_SSP[i_SSP].marital_status_SSP == 'Y')
            printf("Marital Status : Married\n");
        
        else if (pEmployeeRecord_SSP[i_SSP].marital_status_SSP == 'N')
            printf("Marital Status : Unmarried\n");
        else
            printf("Marital Status : Invalid Data Entered\n");
        printf("\n\n");
    }
    if (pEmployeeRecord_SSP)
    {
        free(pEmployeeRecord_SSP);
        pEmployeeRecord_SSP = NULL;
        printf("MEMORY ALLOCATED TO %d EMPLOYEES HAS BEEN SUCCESSFULLY FREED !!!\n\n", num_employees_SSP);
    }
    return(0);
}

// *** SIMPLE RUDIMENTARY IMPLEMENTATION OF gets_s() ***
// *** IMPLEMENTED DUE TO DIFFERENT BEHAVIOUR OF gets_s() / fgets() / fscanf() ON DIFFERENT PLATFORMS ***
// *** BACKSPACE / CHARACTER DELETION AND ARROW KEY CURSOR MOVEMENT NOT IMPLEMENTED ***

void MyGetString(char str[], int str_size)
{
    //variable declarations
    int i_SSP;
    char ch_SSP = '\0';
    
    //code
    i_SSP = 0;
    do
    {
        ch_SSP = getch();
        str[i_SSP] = ch_SSP;
        printf("%c", str[i_SSP]);
        i_SSP++;
    }while ((ch_SSP != '\r') && (i_SSP < str_size));
    if (i_SSP == str_size)
        str[i_SSP - 1] = '\0';
    else
        str[i_SSP] = '\0';
}