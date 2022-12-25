#include<stdio.h>
#include<ctype.h>

#define NUM_EMPLOYEES 5     //Simply change this constant value to have as many records of number of employees you want....
#define NAME_LENGTH 100
#define MARTIAL_STATUS 10

struct Employee
{
    char name_SSP[NAME_LENGTH];
    int age_SSP;
    char sex_SSP;
    float salary_SSP;
    char martial_status_SSP;
};

int main(int argc, char const *argv[])
{
    // functionn prototype
    void MyGetString(char[],int);

    //variable declaration
    struct Employee EmployeeRecord_SSP[NUM_EMPLOYEES];    // An Array of <NUM_EMPLOYEES> structs-Each being type 'struct Employee'*****
    int i_SSP;

    /* code */
    /*User input initializtion of array of 'struct Employee*****/
    for(i_SSP=0;i_SSP<NUM_EMPLOYEES;i_SSP++)
    {
        printf("\n\n");
        printf("********** DATA ENTRY FOR EMPLOYEE NUMBER %d **********\n", (i_SSP + 1));

        printf("\n\n");
        printf("Enter Employee Name : ");
        MyGetString(EmployeeRecord_SSP[i_SSP].name_SSP,NAME_LENGTH);

        printf("\n\n");
        printf("Enter Employee's Age (in years) : ");
        scanf("%d",&EmployeeRecord_SSP[i_SSP].age_SSP);

        printf("\n\n");\
        printf("Enter Employee's Sex (M/m For Male, F/f For Female) : ");
        EmployeeRecord_SSP[i_SSP].sex_SSP=getch();
        printf("%c",EmployeeRecord_SSP[i_SSP].sex_SSP);
        EmployeeRecord_SSP[i_SSP].sex_SSP = toupper(EmployeeRecord_SSP[i_SSP].sex_SSP);

        printf("\n\n");
        printf("Enter Employee's Salary (In Indian Rupees) : ");
        scanf("%d",&EmployeeRecord_SSP[i_SSP].salary_SSP);

        printf("\n\n");
        printf("Is The Employee Married Or Not (Y/y for yes and N/n for no) : ");
        EmployeeRecord_SSP[i_SSP].martial_status_SSP=getch();
        printf("%c",EmployeeRecord_SSP[i_SSP].martial_status_SSP);
        EmployeeRecord_SSP[i_SSP].martial_status_SSP = toupper(EmployeeRecord_SSP[i_SSP].martial_status_SSP);
    }

    // DISPLAY
    printf("\n\n\n\n");
    printf("********** DISPLAYING EMPLOYEE RECORDS **********\n\n");
    for(i_SSP=0;i_SSP<NUM_EMPLOYEES;i_SSP++)
    {
        printf("*********** EMPLOYEE NUMBER %d **********\n\n", (i_SSP + 1));
        
        printf("Name        :%s\n",EmployeeRecord_SSP[i_SSP].name_SSP);
        
        printf("Age         :%d\n",EmployeeRecord_SSP[i_SSP].age_SSP);
        
        if(EmployeeRecord_SSP[i_SSP].sex_SSP == 'M')
        {
            printf("Sex     :Male\n");
        }
        else if(EmployeeRecord_SSP[i_SSP].sex_SSP == 'F')
        {
            printf("Sex     :Female\n");
        }
        else
        {
            printf("Sex     :Invalid Data Entered\n");
        }
        
        if(EmployeeRecord_SSP[i_SSP].martial_status_SSP == 'Y')
        {
            printf("Martial Status      :Married\n");
        }
        else if(EmployeeRecord_SSP[i_SSP].martial_status_SSP == 'N')
        {
            printf("Martial Status      :Unmarried");
        }
        else
        {
            printf("Martial Status      :Inva;lid Statement\n");
        }
    }
    
    return 0;
}

void MyGetString(char str[], int str_size)
{
    // variable declaration
    int i_SSP;
    char ch_SSP = '\0';

    // code 
    i_SSP='\0';
    do
    {
        ch_SSP=getch();
        str[i_SSP]=ch_SSP;
        printf("%c",str[i_SSP]);
        i_SSP++;
    } while ((ch_SSP != '\r') && (i_SSP<str_size));

    if(i_SSP== str_size)
    {
        str[i_SSP-1]='\0';
    }
    else
    {
        str[i_SSP]='\0';
    }
    
}