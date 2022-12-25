#include <stdio.h>
#include<string.h>
//DEFINING STRUCT
struct MyPoint
{
    int x_SSP;
    int y_SSP;
};
//DEFINING STRUCT
struct MyPointProperties
{
    int quadrant_SSP;
    char axis_location_SSP[10];
};

struct MyPoint point_SSP; //declaraing a single variable of type 'struct MyPoint' globally...
struct MyPointProperties point_properties_SSP; //declaraing a single variable of type 'struct MyPointProperties' globally...

int main(void)
{
    //code
    //User Input For The Data Members Of 'struct MyPoint' variable 'point_A'
    printf("\n\n");
    printf("Enter X-Coordinate For A Point : ");
    scanf("%d", &point_SSP.x_SSP);
    printf("Enter Y-Coordinate For A Point : ");
    scanf("%d", &point_SSP.y_SSP);
    
    printf("\n\n"); 
    printf("Point Co-ordinates (x, y) Are : (%d, %d) !!!\n\n", point_SSP.x_SSP, point_SSP.y_SSP);
    
    if (point_SSP.x_SSP == 0 && point_SSP.y_SSP == 0)
        printf("The Point Is The Origin (%d, %d) !!!\n", point_SSP.x_SSP, point_SSP.y_SSP);
    else // Atleast One of the two values (either 'X' or 'Y' or BOTH) is a nonzero value...
    {
        if (point_SSP.x_SSP == 0) // If 'X' IS ZERO...OBVIOUSLY 'Y' IS THE NON-ZERO VALUE
        {
            if (point_SSP.y_SSP < 0) // If 'Y' IS -ve
                strcpy(point_properties_SSP.axis_location_SSP, "Negative Y");
            if (point_SSP.y_SSP > 0) // If 'Y' IS +ve
                strcpy(point_properties_SSP.axis_location_SSP, "Positive Y");

            point_properties_SSP.quadrant_SSP = 0; // A Point Lying On Any Of The Coordinate Axes Is NOT A Part Of ANY Quadrant...
            printf("The Point Lies On The %s Axis !!!\n\n", point_properties_SSP.axis_location_SSP);
        }
        else if (point_SSP.y_SSP == 0) // If 'Y' IS ZERO...OBVIOUSLY 'X' IS THE NON-ZERO VALUE
        {
            if (point_SSP.x_SSP < 0) // If 'X' IS -ve
                strcpy(point_properties_SSP.axis_location_SSP, "Negative X");
            if (point_SSP.x_SSP > 0) // If 'X' IS +ve
                strcpy(point_properties_SSP.axis_location_SSP, "Positive X");
    
            point_properties_SSP.quadrant_SSP = 0; // A Point Lying On Any Of The Coordinate Axes Is NOT A Part Of ANY Quadrant...
            printf("The Point Lies On The %s Axis !!!\n\n",point_properties_SSP.axis_location_SSP);
        }           
        else // BOTH 'X' AND 'Y' ARE NON-ZERO
        {
            point_properties_SSP.axis_location_SSP[0] = '\0'; // A Point Lying In ANY Of The 4 Quadrants Cannot Be Lying On Any Of The Co-ordinate Axes...
            
            if (point_SSP.x_SSP > 0 && point_SSP.y_SSP > 0) // 'X' IS +ve AND 'Y' IS +ve
                point_properties_SSP.quadrant_SSP = 1;
            else if (point_SSP.x_SSP < 0 && point_SSP.y_SSP > 0) // 'X' IS -ve AND 'Y' IS +ve
                point_properties_SSP.quadrant_SSP = 2;
            else if (point_SSP.x_SSP < 0 && point_SSP.y_SSP < 0) // 'X' IS -ve AND 'Y' IS -ve
                point_properties_SSP.quadrant_SSP = 3;
            else // 'X' IS +ve AND 'Y' IS -ve
                point_properties_SSP.quadrant_SSP = 4;
            printf("The Point Lies In Quadrant Number %d !!!\n\n", point_properties_SSP.quadrant_SSP);
        }
    }
    return(0);
}