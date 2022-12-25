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

    if(option_SSP=='A' || option_SSP=='a')
    {
        result_SSP=a_SSP+b_SSP;
        printf("Addition Of A =%d And B=%d gives Result =%d",a_SSP,b_SSP,result_SSP);
    }

    else if(option_SSP=='S' || option_SSP=='s')
    {
        if(a_SSP>b_SSP)
        {
            result_SSP=a_SSP-b_SSP;
            printf("Subtraction of B=%d from A=%d Gives Result=%d !!!...\n\n ",a_SSP,b_SSP,result_SSP);
        }
        else
        {
            result_SSP=b_SSP-a_SSP;
            printf("Subtraction of A=%d From B=%d Gives Result=%d !!!...\n\n ",b_SSP,a_SSP,result_SSP);
        }
    }
    else if(option_SSP=='M' || option_SSP=='m')
    {
        result_SSP=a_SSP*b_SSP;
        printf("Multiplication of A=%d And B=%d Gives Result=%d !!!...\n\n ",a_SSP,b_SSP,result_SSP);
    }
    else if(option_SSP=='D' || option_SSP=='d')
    {
        printf("enter Option In Character: \n\n");
        printf("'Q' or 'q' or '/' For Quotient Upon Division: \n");
        printf("'R' or 'r' or '%%' For remainder upon division: \n");

        printf("Enter Option: ");
        option_division_SSP=getch();

        printf("\n\n");

        if(option_division_SSP=='Q' || option_division_SSP=='q' || option_division_SSP=='/')
        {
            if(a_SSP>=b_SSP)
            {
                result_SSP=a_SSP/b_SSP;
                printf("Divison of A=%d And B=%d Gives Quotient=%d \n",a_SSP,b_SSP,result_SSP);
            }
        
            else
            {
                result_SSP=b_SSP/a_SSP;
                printf("Divison of B=%d And A=%d Gives Quotient=%d \n",b_SSP,a_SSP,result_SSP);
            }
        }
        else if(option_division_SSP=='R' || option_division_SSP=='r' || option_division_SSP=='%')
        {
            if(a_SSP>=b_SSP)
            {
                result_SSP=a_SSP/b_SSP;
                printf("Divison of A=%d And B=%d Gives Remainder=%d \n",a_SSP,b_SSP,result_SSP);
            }
            else
            {
                result_SSP=b_SSP/a_SSP;
                printf("Divison of B=%d And A=%d Gives Remainder=%d \n",b_SSP,a_SSP,result_SSP);
            } 
        }   
        else 
        {
                printf("Invalid Character %c Entered For division !!! Please try again...\n\n",option_division_SSP);
        }
    }
    else
    {
        printf("Invalid Character %c Entered !!! Please Try Again...\n\n",option_SSP);
    }
    printf("If-Else-If ladder Complete !!!\n");

    return 0;
}
/*
Enter Value For 'A': 20
Enter Value for 'B': 40
Enter Option In Character: 

'A' or'a' for addition :
'S' or 's' for Subtraction :
'M' or 'm' for Multiplication :
'D' or 'd' for division :

Enter Option:

Addition Of A =20 And B=40 gives Result =60If-Else-If ladder Complete !!!
*/