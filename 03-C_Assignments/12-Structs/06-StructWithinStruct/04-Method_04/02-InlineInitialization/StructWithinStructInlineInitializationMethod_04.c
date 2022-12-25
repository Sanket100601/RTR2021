#include<stdio.h>

int main(void)
{
    // variable declaration
    int length_SSP,breadth_SSP,area_SSP;

    struct MyPoint
    {
        int x_SSP;
        int y_SSP;
    };

    struct Rectangle
    {
        struct MyPoint point_01;
        struct MyPoint point_02;
    };
    
    struct Rectangle rect={{2,3},{5,6}};
    // Code
    length_SSP = rect.point_02.y_SSP - rect.point_01.y_SSP;
    if(length_SSP<0)
    {
        length_SSP = length_SSP * -1;
    }
    
    breadth_SSP = rect.point_02.x_SSP - rect.point_01.x_SSP;
    if(breadth_SSP<0)
    {
        breadth_SSP = breadth_SSP * -1;
    }

    area_SSP = length_SSP *breadth_SSP;

    printf("\n\n");
    printf("Length Of Rectangle = %d\n\n", length_SSP);
    printf("Breadth Of Rectangle = %d\n\n", breadth_SSP);
    printf("Area Of Rectangle = %d\n\n", area_SSP);
    
    return(0);
}