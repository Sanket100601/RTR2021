#include <stdio.h>
struct MyNumber
{
    int num_SSP;
    int num_table_SSP[10];
};

struct NumTables
{
    struct MyNumber a_SSP;
    struct MyNumber b_SSP;
    struct MyNumber c_SSP;
};

int main(void)
{
    //variable declarations
    struct NumTables tables_SSP;
    int i_SSP;
    
    //code
    tables_SSP.a_SSP.num_SSP = 2;
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        tables_SSP.a_SSP.num_table_SSP[i_SSP] = tables_SSP.a_SSP.num_SSP * (i_SSP + 1);
    printf("\n\n");
    printf("Table Of %d : \n\n", tables_SSP.a_SSP.num_SSP);
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("%d * %d = %d\n", tables_SSP.a_SSP.num_SSP, (i_SSP + 1), tables_SSP.a_SSP.num_table_SSP[i_SSP]);
    
    tables_SSP.b_SSP.num_SSP = 3;
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        tables_SSP.b_SSP.num_table_SSP[i_SSP] = tables_SSP.b_SSP.num_SSP * (i_SSP + 1);
    printf("\n\n");
    printf("Table Of %d : \n\n", tables_SSP.b_SSP.num_SSP);
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("%d * %d = %d\n", tables_SSP.b_SSP.num_SSP, (i_SSP + 1), tables_SSP.b_SSP.num_table_SSP[i_SSP]);

    tables_SSP.c_SSP.num_SSP = 4;
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        tables_SSP.c_SSP.num_table_SSP[i_SSP] = tables_SSP.c_SSP.num_SSP * (i_SSP + 1);
    printf("\n\n");
    printf("Table Of %d : \n\n", tables_SSP.c_SSP.num_SSP);
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
        printf("%d * %d = %d\n", tables_SSP.c_SSP.num_SSP, (i_SSP + 1), tables_SSP.c_SSP.num_table_SSP[i_SSP]);
    
    return(0);
}