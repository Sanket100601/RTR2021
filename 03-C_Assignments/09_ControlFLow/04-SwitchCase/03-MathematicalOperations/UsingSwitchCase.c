#include<stdio.h>   //for printf()
#include<stdio.h>   //for getch()

int main(void)
{
    //variable declaration 
    int a_SSP,b_SSP;
    int result_SSP;

    char option_SSP,option_division_SSP;

    //code
    printf("\n\n");

    printf("Enter Value For 'A': ");
    scanf("%d",&a_SSP);

    printf("Enter Value for 'B': ");
    scanf("%d",&b_SSP);

    printf("Enter Option In Character: \n\n");
    printf("'A' or'a' for addition : \n");
    printf("'S' or 's' for Subtraction : \n");
    printf("'M' or 'm' for Multiplication : \n");
    printf("'D' or 'd' for division : \n\n");

    printf("Enter Option: ");
    option_SSP=getch();

    printf("\n\n");

    switch(option_SSP)
    {
    //Fall Through Condition For 'A' and 'a'
    case 'A': 
    case 'a':
        result_SSP=a_SSP+b_SSP;
        printf("Addition of A=%d And B=%d Gives Result %d !!!\n\n",a_SSP,b_SSP,result_SSP);
        break;

    //Fall Through condition for 'S' and 's'
    case 'S':
    case 's':
        if(a_SSP>=b_SSP)
        {
            result_SSP=a_SSP-b_SSP;
            printf("Subtraction of B=%d From a=%d Gives Result %d !!!\n\n",b_SSP,a_SSP,result_SSP);
        }
        else
        {
            result_SSP=b_SSP-a_SSP;
            printf("Subtraction Of A=%d From B=%d Gives Result %d !!!\n\n",a_SSP,b_SSP,result_SSP);
        }
        break;
    
    //Fall through condition for 'M' and 'm'
    case 'M':
    case 'm':
        result_SSP=a_SSP*b_SSP;
        printf("Multiplication Of A=%d and b=%d Gives Result %d !!!\n\n",a_SSP,b_SSP,result_SSP);
        break;
    
    //Fall Through condition For 'D' and 'd'
    case 'D':
    case 'd':
        printf("'Q' or 'q' or '/' For Quotient Upon Division: \n");
        printf("'R' or 'r' or '%%' For remainder upon division: \n");

        printf("Enter Option: ");
        option_division_SSP=getch();

        printf("\n\n");

        switch (option_division_SSP)
        {
        //Fall Through Condition for 'Q' and 'q' and '/'
        case 'Q':
        case 'q':
        case '/':
            if(a_SSP>=b_SSP)
            {
                result_SSP=a_SSP/b_SSP;
                printf("Division Of A=%d By B=%d Gives Quotient =%d !!!\n",a_SSP,b_SSP,result_SSP);
            }
            else
            {
                result_SSP=b_SSP/a_SSP;
                printf("Division Of B=%d By A=%d Gives Quotient =%d !!!\n",b_SSP,a_SSP,result_SSP);
            }
            break;
        //Fall Through Condition 
        case 'R':
        case 'r':
        case '%':
            if(a_SSP>=b_SSP)
            {
                result_SSP=a_SSP%b_SSP;
                printf("Division Of A=%d By B=%d Gives Remainder =%d !!!\n",a_SSP,b_SSP,result_SSP);
            }
            else
            {
                result_SSP=b_SSP/a_SSP;
                printf("Division Of B=%d By A=%d Gives Remainder =%d !!!\n",b_SSP,a_SSP,result_SSP);
            }
            break;
        default: 
            printf("Invalid Character %c Entered For Division !!! Please Try Again...\n\n",option_division_SSP);
            break;
        }
    default:
        printf("Ivalid Character Entered !!! Please try again...\n\n",option_SSP);
        break;
    }

    printf("Switch Case Block Complete !!!\n");

    return 0;
}
/*
Enter Value For 'A': 12
Enter Value for 'B': 34
Enter Option In Character: 

'A' or'a' for addition :
'S' or 's' for Subtraction :
'M' or 'm' for Multiplication :
'D' or 'd' for division :

Enter Option:

'Q' or 'q' or '/' For Quotient Upon Division: 
'R' or 'r' or '%' For remainder upon division:
Enter Option:

Division Of B=34 By A=12 Gives Quotient =2 !!!
Ivalid Character Entered !!! Please try again...

Switch Case Block Complete !!!
*/
