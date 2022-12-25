#include <stdio.h>
struct MyPoint
{
    int x_SSP;
    int y_SSP;
};
struct Rectangle
{
    struct MyPoint point_01, point_02;
};

int main(void)
{
    //variable declarations
    int length_SSP, breadth_SSP, area_SSP;
    struct Rectangle rect;
    
    //code
    printf("\n\n");
    printf("Enter Leftmost X-Coordinate Of Rectangle : ");
    scanf("%d", &rect.point_01.x_SSP);

    printf("\n\n");
    printf("Enter Bottommost Y-Coordinate Of Rectangle : ");
    scanf("%d", &rect.point_01.y_SSP);

    printf("\n\n");
    printf("Enter Rightmost X-Coordinate Of Rectangle : ");
    scanf("%d", &rect.point_02.x_SSP);

    printf("\n\n");
    printf("Enter Topmost Y-Coordinate Of Rectangle : ");
    scanf("%d", &rect.point_02.y_SSP);

    length_SSP = rect.point_02.y_SSP - rect.point_01.y_SSP;
    if (length_SSP < 0)
        length_SSP = length_SSP * -1;

    breadth_SSP = rect.point_02.x_SSP - rect.point_01.x_SSP;    
    if (breadth_SSP < 0)
        breadth_SSP = breadth_SSP * -1;

    area_SSP = length_SSP * breadth_SSP;
   
    printf("\n\n"); 
    printf("Length Of Rectangle = %d\n\n", length_SSP);
    printf("Breadth Of Rectangle = %d\n\n", breadth_SSP);
    printf("Area Of Rectangle = %d\n\n", area_SSP);
    
    return(0);
}