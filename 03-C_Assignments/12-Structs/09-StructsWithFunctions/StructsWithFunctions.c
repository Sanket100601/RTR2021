#include <stdio.h>
// DEFINING STRUCT
struct MyData
{
    int i_SSP;
    float f_SSP;
    double d_SSP;
    char c_SSP;
};
int main(void)
{
    //function prototype
    struct MyData AddStructMembers(struct MyData, struct MyData, struct MyData);

    //variable declarations
    struct MyData data1_SSP, data2_SSP, data3_SSP, answer_data_SSP;
  
    //code
    // *** data1 ***
    printf("\n\n\n\n");
    printf("********** DATA 1 **********\n\n");
    printf("Enter Integer Value For 'i' Of 'struct MyData data1' : ");
    scanf("%d", &data1_SSP.i_SSP);
 
    printf("\n\n");
    printf("Enter Floating-Point Value For 'f' Of 'struct MyData data1' : ");
    scanf("%f", &data1_SSP.f_SSP);
 
    printf("\n\n");
    printf("Enter 'Double' Value For 'd' Of 'struct MyData data1' : ");
    scanf("%lf", &data1_SSP.d_SSP);
 
    printf("\n\n");
    printf("Enter Character Value For 'c' Of 'struct MyData data1' : ");
    data1_SSP.c_SSP = getch();
    printf("%c", data1_SSP.c_SSP);
  
    // *** data2 ***
    printf("\n\n\n\n");
    printf("********** DATA 2 **********\n\n");
    printf("Enter Integer Value For 'i' Of 'struct MyData data2' : ");
    scanf("%d", &data2_SSP.i_SSP);
  
    printf("\n\n");
    printf("Enter Floating-Point Value For 'f' Of 'struct MyData data2' : ");
    scanf("%f", &data2_SSP.f_SSP);
  
    printf("\n\n");
    printf("Enter 'Double' Value For 'd' Of 'struct MyData data2' : ");
    scanf("%lf", &data2_SSP.d_SSP);
  
    printf("\n\n");
    printf("Enter Character Value For 'c' Of 'struct MyData data2' : ");
    data2_SSP.c_SSP = getch();
    printf("%c", data2_SSP.c_SSP);
  
    // *** data3 ***
    printf("\n\n\n\n");
    printf("********** DATA 3 **********\n\n");
    printf("Enter Integer Value For 'i' Of 'struct MyData data3' : ");
    scanf("%d", &data3_SSP.i_SSP);
  
    printf("\n\n");
    printf("Enter Floating-Point Value For 'f' Of 'struct MyData data3' : ");
    scanf("%f", &data3_SSP.f_SSP);
  
    printf("\n\n");
    printf("Enter 'Double' Value For 'd' Of 'struct MyData data3' : ");
    scanf("%lf", &data3_SSP.d_SSP);
  
    printf("\n\n");
    printf("Enter Character Value For 'c' Of 'struct MyData data3' : ");
    data3_SSP.c_SSP = getch();
    printf("%c", data3_SSP.c_SSP);
  
    // *** CALLING FUNCTION AddStructMembers() WHICH ACCEPTS THREE VARIABLES OF 
    //TYPE 'struct MyData' AS PARAMETERS AND ADDS UP THE RESPECTIVE MEMBERS AND 
    //RETURNS THE ANSWER IN ANOTHER struct OF SAME TYPE ***
    answer_data_SSP = AddStructMembers(data1_SSP, data2_SSP, data3_SSP);
  
    printf("\n\n\n\n");
    printf("********** ANSWER *********\n\n");
    printf("answer_data.i = %d\n", answer_data_SSP.i_SSP);
    printf("answer_data.f = %f\n", answer_data_SSP.f_SSP);
    printf("answer_data.d = %lf\n\n", answer_data_SSP.d_SSP);
  
    answer_data_SSP.c_SSP = data1_SSP.c_SSP;
    printf("answer_data.c (from data1) = %c\n\n", answer_data_SSP.c_SSP);
  
    answer_data_SSP.c_SSP = data2_SSP.c_SSP;
    printf("answer_data.c (from data2) = %c\n\n", answer_data_SSP.c_SSP);
  
    answer_data_SSP.c_SSP = data3_SSP.c_SSP;
    printf("answer_data.c (from data3) = %c\n\n", answer_data_SSP.c_SSP);
  
    return(0);
}
struct MyData AddStructMembers(struct MyData md_one, struct MyData md_two, struct MyData md_three)
{
    //variable declarations
    struct MyData answer_SSP;
    //code
    answer_SSP.i_SSP = md_one.i_SSP + md_two.i_SSP + md_three.i_SSP;
    answer_SSP.f_SSP = md_one.f_SSP + md_two.f_SSP + md_three.f_SSP;
    answer_SSP.d_SSP = md_one.d_SSP + md_two.d_SSP + md_three.d_SSP;
    return(answer_SSP);
}