#include<stdio.h>

#define MAX_NUMBER(a_SSP,b_SSP)((a_SSP>b_SSP) ? a_SSP : b_SSP)

int main(int argc,char*agv[],char*envp[])
{
    //variable declarartion
    int iNum_01_SSP;
    int iNum_02_SSP;
    int iResult_SSP;
    
    float fNum_01_SSP;
    float fNum_02_SSP;
    float fResult_SSP;

    //code
    //Comparing Integer Values
    printf("\n\n");
    printf("Enter An integer Number : \n\n");
    scanf("%d",&iNum_01_SSP);

    printf("\n\n");
    printf("Enter Another integer Number : \n\n");
    scanf("%d",&iNum_02_SSP);
    
    iResult_SSP = MAX_NUMBER(iNum_01_SSP,iNum_02_SSP);
    printf("\n\n");
    printf("Result Of Macro Function MAX_FUNCTION() = %d\n",iResult_SSP);

    printf("\n\n");

    //'COMPARING FLOATING-POINT VALUES
    printf("\n\n");
    printf("Enter a floating point number : \n\n");
    scanf("%f",&fNum_01_SSP);

    printf("\n\n");
    printf("Enter Another Floating Number : \n\n");
    scanf("%d",&fNum_02_SSP);
    
    fResult_SSP = MAX_NUMBER(fNum_01_SSP,fNum_02_SSP);
    printf("\n\n");
    printf("Result Of Macro Function MAX_FUNCTION() = %d\n",fResult_SSP);

    printf("\n\n");

    return 0;
}
/*
Enter An integer Number : 

25


Enter Another integer Number :

20


Result Of Macro Function MAX_FUNCTION() = 25




Enter a floating point number :

1.3


Enter Another Floating Number :

1.56


Result Of Macro Function MAX_FUNCTION() = -1073741824
*/
