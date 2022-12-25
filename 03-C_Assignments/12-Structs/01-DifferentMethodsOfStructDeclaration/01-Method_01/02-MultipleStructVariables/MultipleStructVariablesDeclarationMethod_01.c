#include<stdio.h>

// Defining Struct 
struct MyPoint
{
    int x_SSP;
    int y_SSP;
} point_A,point_B,point_C,point_D,point_E;  // Declaring 5 struct variable of type 'struct MyPoint' globally...

int main(void)
{
    // code
    // Assigning Data Values To Data members of 'struct MyPoint' variable 'point_A'
    point_A.x_SSP=3;
    point_A.y_SSP=0;

    // Assigning Data Value to data members of 'struct Mypoint' variable 'point_B'
    point_B.x_SSP=1;
    point_B.y_SSP=2;

    // Assigning Data Value to data members of 'struct Mypoint' variable 'point_C'
    point_C.x_SSP=1;
    point_C.y_SSP=2;

    // Assigning Data Value to data members of 'struct Mypoint' variable 'point_D'
    point_D.x_SSP=1;
    point_D.y_SSP=2;
    
    // Assigning Data Value to data members of 'struct Mypoint' variable 'point_E'
    point_E.x_SSP=1;
    point_E.y_SSP=2;

    //Displaying Values To the data members of 'struct MyPoint' (all variables)
    printf("\n\n");
    printf("Co-ordinates (x,y) Of Point 'A' are : (%d,%d)\n\n",point_A.x_SSP,point_A.y_SSP);
    printf("Co-ordinates (x,y) Of Point 'B' are : (%d,%d)\n\n",point_B.x_SSP,point_B.y_SSP);
    printf("Co-ordinates (x,y) Of Point 'C' are : (%d,%d)\n\n",point_C.x_SSP,point_C.y_SSP);
    printf("Co-ordinates (x,y) Of Point 'D' are : (%d,%d)\n\n",point_D.x_SSP,point_D.y_SSP);
    printf("Co-ordinates (x,y) Of Point 'E' are : (%d,%d)\n\n",point_E.x_SSP,point_E.y_SSP);

    return 0;
}