#include<stdio.h>
int main(void)
{
    //variable declarations
    int num_month_SSP;

    //code
    printf("\n\n");

    printf("Enter the Number Of Month (1 to 12) : ");
    scanf("%d",&num_month_SSP);

    printf("\n\n");

    switch (num_month_SSP)
    {
    case 1: //like 'if'
        printf("Month Number %d Is January !!!\n\n",num_month_SSP);
        break;
        
    case 2: //like 'else' 'if'
        printf("Month Number %d Is February !!!\n\n",num_month_SSP);
        break;
    
    case 3: //like 'else' 'if'
        printf("Month Number %d Is March !!!\n\n",num_month_SSP);
        break;

    case 4: //like 'else' 'if'
        printf("Month Number %d Is April !!!\n\n",num_month_SSP);
        break;
    
    case 5: //like 'else' 'if'
        printf("Month Number %d Is May !!!\n\n",num_month_SSP);
        break;
    
    case 6: //like 'else' 'if'
        printf("Month Number %d Is June !!!\n\n",num_month_SSP);
        break;

    case 7: //like 'else' 'if'
        printf("Month Number %d Is July !!!\n\n",num_month_SSP);
        break;
    
    case 8: //like 'else' 'if'
        printf("Month Number %d Is August !!!\n\n",num_month_SSP);
        break;

    case 9: //like 'else' 'if'
        printf("Month Number %d Is September !!!\n\n",num_month_SSP);
        break;
    
    case 10: //like 'else' 'if'
        printf("Month Number %d Is October !!!\n\n",num_month_SSP);
        break;
    
    case 11: //like 'else' 'if'
        printf("Month Number %d Is November !!!\n\n",num_month_SSP);
        break;
    
    case 12: //like 'else' 'if'
        printf("Month Number %d Is December !!!\n\n",num_month_SSP);
        break;
    
    default:
        printf("Invalid Month Number %d Entered !!! Please Try Again...\n\n",num_month_SSP);
        break;
    }

    printf("Switch Case Block Complete");

    return 0;
}
/*
Enter the Number Of Month (1 to 12) : 6


Month Number 6 Is June !!!

Switch Case Block Complete
*/