#include<stdio.h>

// Defininng Struct
struct MyPoint
{
    int x_SSP;
    int y_SSP;
};

struct MyPoint point_A_SSP,point_B_SSP,point_C_SSP,point_D_SSP,point_E_SSP;     // Declaring the 5 struct variables of 'struct MyPoint' globally...

int main(void)
{
    // code 
    // Assigning Data Values To The Data Member Of 'struct MyPoint' variable 'point_A'
    point_A_SSP.x_SSP=3;
    point_A_SSP.y_SSP=0;
    
    // Assigning data Values To The Data Members Of 'Struct MyPoint' variable point_B
    point_B_SSP.x_SSP=1;
    point_B_SSP.y_SSP=2;

    // Assigning Data Values To The Data Member Of 'struct MyPoint' variable point_C
    point_C_SSP.x_SSP=9;
    point_C_SSP.y_SSP=6;

    // Assigning Data Values To The Data Member Of 'struct MyPoint' variable point_D
    point_D_SSP.x_SSP=8;
    point_D_SSP.y_SSP=2;

    // Assigning Data Values To The Data Member Of 'struct MyPoint' variable point_E
    point_E_SSP.x_SSP=11;
    point_E_SSP.y_SSP=8;
    
    //Displaying Values Of The Data Members Of 'struct MyPoint' (all variables)
    printf("\n\n");
    printf("Co-ordinates (x, y) Of Point 'A' Are : (%d, %d)\n\n", point_A_SSP.x_SSP, point_A_SSP.y_SSP);
    printf("Co-ordinates (x, y) Of Point 'B' Are : (%d, %d)\n\n", point_B_SSP.x_SSP, point_B_SSP.y_SSP);
    printf("Co-ordinates (x, y) Of Point 'C' Are : (%d, %d)\n\n", point_C_SSP.x_SSP, point_C_SSP.y_SSP);
    printf("Co-ordinates (x, y) Of Point 'D' Are : (%d, %d)\n\n", point_D_SSP.x_SSP, point_D_SSP.y_SSP);
    printf("Co-ordinates (x, y) Of Point 'E' Are : (%d, %d)\n\n", point_E_SSP.x_SSP, point_E_SSP.y_SSP);

    return 0;
}