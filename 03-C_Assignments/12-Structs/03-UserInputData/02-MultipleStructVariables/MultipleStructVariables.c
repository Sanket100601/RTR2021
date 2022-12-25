#include <stdio.h>
struct MyPoint
{
    int x_SSP;
    int y_SSP;
};

int main(void)
{
    //variable declarations
    struct MyPoint point_A_SSP, point_B_SSP, point_C_SSP, point_D_SSP, point_E_SSP; //declaring a 5 struct variables

    //code
    //User Input For The Data Members Of 'struct MyPoint' variable 'point_A'
    printf("\n\n");
    printf("Enter X-Coordinate For Point 'A' : ");
    scanf("%d", &point_A_SSP.x_SSP);
    printf("Enter Y-Coordinate For Point 'A' : ");
    scanf("%d", &point_A_SSP.y_SSP);
 
    //User Input For The Data Members Of 'struct MyPoint' variable 'point_B'
    printf("\n\n");
    printf("Enter X-Coordinate For Point 'B' : ");
    scanf("%d", &point_B_SSP.x_SSP);
    printf("Enter Y-Coordinate For Point 'B' : ");
    scanf("%d", &point_B_SSP.y_SSP);
    
    //User Input For The Data Members Of 'struct MyPoint' variable 'point_C'
    printf("\n\n");
    printf("Enter X-Coordinate For Point 'C' : ");
    scanf("%d", &point_C_SSP.x_SSP);
    printf("Enter Y-Coordinate For Point 'C' : ");
    scanf("%d", &point_C_SSP.y_SSP);

    //User Input For The Data Members Of 'struct MyPoint' variable 'point_D'
    printf("\n\n");
    printf("Enter X-Coordinate For Point 'D' : ");
    scanf("%d", &point_D_SSP.x_SSP);
    printf("Enter Y-Coordinate For Point 'D' : ");
    scanf("%d", &point_D_SSP.y_SSP);
    
    //User Input For The Data Members Of 'struct MyPoint' variable 'point_E'
    printf("\n\n");
    printf("Enter X-Coordinate For Point 'E' : ");
    scanf("%d", &point_E_SSP.x_SSP);
    printf("Enter Y-Coordinate For Point 'E' : ");
    scanf("%d", &point_E_SSP.y_SSP);

    //Displaying Values Of The Data Members Of 'struct MyPoint' (all variables)
    printf("\n\n");
    printf("Co-ordinates (x, y) Of Point 'A' Are : (%d, %d)\n\n", point_A_SSP.x_SSP, point_A_SSP.y_SSP);
    printf("Co-ordinates (x, y) Of Point 'B' Are : (%d, %d)\n\n", point_B_SSP.x_SSP, point_B_SSP.y_SSP);
    printf("Co-ordinates (x, y) Of Point 'C' Are : (%d, %d)\n\n", point_C_SSP.x_SSP, point_C_SSP.y_SSP);
    printf("Co-ordinates (x, y) Of Point 'D' Are : (%d, %d)\n\n", point_D_SSP.x_SSP, point_D_SSP.y_SSP);
    printf("Co-ordinates (x, y) Of Point 'E' Are : (%d, %d)\n\n", point_E_SSP.x_SSP, point_E_SSP.y_SSP);

    return(0);
}