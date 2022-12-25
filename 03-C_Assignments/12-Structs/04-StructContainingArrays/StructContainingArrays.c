#include<stdio.h>

#define FLOAT_ARRAY_SIZE 5
#define INT_ARRAY_SIZE 10
#define CHAR_ARRAY_SIZE 26

#define NUM_STRINGS 10
#define MAX_CHARACTERS_PER_STRINGS 65   //'A'

#define ALPHABET_BEGINNING

struct MyDataOne 
{
    int iArray_SSP[INT_ARRAY_SIZE];
    float fArray_SSP[FLOAT_ARRAY_SIZE];
};

struct MyDataTwo
{
    char cArray_SSP[CHAR_ARRAY_SIZE];
    char strArray_SSP[NUM_STRINGS][MAX_CHARACTERS_PER_STRINGS];
};

int main(void)
{
    // variable declaration 
    struct MyDataOne data_one_SSP;
    struct MyDataTwo data_two_SSP;
    int i_SSP;

    // code
    data_one_SSP.fArray_SSP[0]=0.1f;
    data_one_SSP.fArray_SSP[1]=1.2f;
    data_one_SSP.fArray_SSP[2]=2.3f;
    data_one_SSP.fArray_SSP[3]=3.4f;
    data_one_SSP.fArray_SSP[4]=4.5f;

    printf("\n\n");
    printf("Enter %d Integers : \n\n",INT_ARRAY_SIZE);
    for(i_SSP=0;i_SSP<INT_ARRAY_SIZE;i_SSP++)
    {
        scanf("%d",&data_one_SSP.iArray_SSP[i_SSP]);
    }

    for(i_SSP=0;i_SSP<CHAR_ARRAY_SIZE;i_SSP++)
    {
        data_two_SSP.cArray_SSP[i_SSP]=(char)(ALPHABET_BEGINNING+i_SSP);
    }

    strcpy(data_two_SSP.strArray_SSP[0],"WELCOME !!!");
    strcpy(data_two_SSP.strArray_SSP[1],"THIS");
    strcpy(data_two_SSP.strArray_SSP[2],"IS");
    strcpy(data_two_SSP.strArray_SSP[3],"ASTROMEDICOMP");
    strcpy(data_two_SSP.strArray_SSP[4],"REAL");
    strcpy(data_two_SSP.strArray_SSP[5],"TIME");
    strcpy(data_two_SSP.strArray_SSP[6],"RENDERING");
    strcpy(data_two_SSP.strArray_SSP[7],"BATCH");
    strcpy(data_two_SSP.strArray_SSP[8],"OF");
    strcpy(data_two_SSP.strArray_SSP[9],"2020-21");

    // *** DISPLAYING DATA MEMBERS OF 'struct DataOne' AND THEIR VALUES ***
    printf("\n\n");
    printf("Members Of 'struct DataOne' Alongwith Their Assigned Values Are : \n\n");
    
    printf("\n\n");
    printf("Integer Array (data_one.iArray[]) : \n\n");
    for (i_SSP = 0; i_SSP < INT_ARRAY_SIZE; i_SSP++)
        printf("data_one.iArray[%d] = %d\n", i_SSP, data_one_SSP.iArray_SSP[i_SSP]);

    printf("\n\n");
    printf("Floating-Point Array (data_one.fArray[]) : \n\n");
    for (i_SSP = 0; i_SSP < FLOAT_ARRAY_SIZE; i_SSP++)
        printf("data_one.fArray[%d] = %f\n", i_SSP, data_one_SSP.fArray_SSP[i_SSP]);

    // *** DISPLAYING DATA MEMBERS OF 'struct DataTwo' AND THEIR VALUES ***
    printf("\n\n");
    printf("Members Of 'struct DataTwo' Alongwith Their Assigned Values Are : \n\n");

    printf("\n\n");
    printf("Character Array (data_two.cArray[]) : \n\n");
    for (i_SSP = 0; i_SSP < CHAR_ARRAY_SIZE; i_SSP++)
        printf("data_two.cArray[%d] = %c\n", i_SSP, data_two_SSP.cArray_SSP[i_SSP]);

    printf("\n\n");
    printf("String Array (data_two.strArray[]) : \n\n");
    for (i_SSP = 0; i_SSP < NUM_STRINGS; i_SSP++)
        printf("%s ", data_two_SSP.strArray_SSP[i_SSP]);
    
    printf("\n\n");

    return(0);
}