#include <stdio.h>
struct MyNumber
{
    int num_SSP;
    int num_table_SSP[10];
};
struct NumTables
{
    struct MyNumber n_SSP;
};


int main(void)
{
    //variable declarations
    struct NumTables tables_SSP[10]; //an array of 10 'struct NumTables'
    int i_SSP, j_SSP;

    //code
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    {
        tables_SSP[i_SSP].n_SSP.num_SSP = (i_SSP + 1);
    }
    for (i_SSP = 0; i_SSP < 10; i_SSP++)
    {
        printf("\n\n");
        printf("Table Of %d : \n\n", tables_SSP[i_SSP].n_SSP.num_SSP);
        for (j_SSP = 0; j_SSP < 10; j_SSP++)
        {
            tables_SSP[i_SSP].n_SSP.num_table_SSP[j_SSP] = tables_SSP[i_SSP].n_SSP.num_SSP * (j_SSP + 1);
            printf("%d * %d = %d\n", tables_SSP[i_SSP].n_SSP.num_SSP, (j_SSP + 1), tables_SSP[i_SSP].n_SSP.num_table_SSP[j_SSP]);
        }
    }
    return(0);
}