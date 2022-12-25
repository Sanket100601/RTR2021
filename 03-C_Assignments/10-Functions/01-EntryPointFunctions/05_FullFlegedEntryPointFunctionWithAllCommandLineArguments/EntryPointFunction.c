#include<stdio.h>   //'stdio.h' contains declaration of printf()

//Entry point function=> main()=>Valid Return Type(int) and 3 parameter (int argc,char*argv[],char*envp[])
int main(int argc,char*argv[],char*envp[])
{
    int i_SSP;

    //code
    printf("\n\n");
    printf("Hello World !!!\n");    //Library Function
    printf("Number of  command Line Argumrnts =%d\n\n",argc);

    printf("Command line Arguments Passed To this Program Are : \n\n");
    for ( i_SSP = 0; i_SSP < argc; i_SSP++)
    {
        /* code */
        printf("Command Line Argument Number %d = %s\n",(i_SSP+1),argv[i_SSP]);
    }
    printf("\n\n");

    printf("First 20 Environmental variable Passed To This Program Are : \n\n");
    for ( i_SSP = 0; i_SSP < 20; i_SSP++)
    {
        /* code */
        printf("Environment Variable Number %d = %s\n",(i_SSP+1),envp[i_SSP]);
    }
    
    return 0;
}
/*
Hello World !!!
Number of  command Line Argumrnts =1

Command line Arguments Passed To this Program Are :

Command Line Argument Number 1 = C:\MyProject\RTR\03-C_Assignments\10-Functions\01-EntryPointFunctions\05_FullFlegedEntryPointFunctionWithAllCommandLineArguments\EntryPointFunction.exe


First 20 Environmental variable Passed To This Program Are :

Environment Variable Number 1 = ALLUSERSPROFILE=C:\ProgramData
Environment Variable Number 2 = APPDATA=C:\Users\sanke\AppData\Roaming
Environment Variable Number 3 = CHROME_CRASHPAD_PIPE_NAME=\\.\pipe\crashpad_10912_DZSQGFCTIXSKGRUB
Environment Variable Number 4 = CommonProgramFiles=C:\Program Files (x86)\Common Files
Environment Variable Number 5 = CommonProgramFiles(x86)=C:\Program Files (x86)\Common Files
Environment Variable Number 6 = CommonProgramW6432=C:\Program Files\Common Files
Environment Variable Number 7 = COMPUTERNAME=SANKETPC
Environment Variable Number 8 = ComSpec=C:\WINDOWS\system32\cmd.exe
Environment Variable Number 9 = DriverData=C:\Windows\System32\Drivers\DriverData
Environment Variable Number 10 = FPS_BROWSER_APP_PROFILE_STRING=Internet Explorer
Environment Variable Number 11 = FPS_BROWSER_USER_PROFILE_STRING=Default
Environment Variable Number 12 = HOMEDRIVE=C:
Environment Variable Number 13 = HOMEPATH=\Users\sanke
Environment Variable Number 14 = LOCALAPPDATA=C:\Users\sanke\AppData\Local
Environment Variable Number 15 = LOGONSERVER=\\SANKETPC
Environment Variable Number 16 = NUMBER_OF_PROCESSORS=8
Environment Variable Number 17 = OneDrive=C:\Users\sanke\OneDrive
Environment Variable Number 18 = OneDriveConsumer=C:\Users\sanke\OneDrive
Environment Variable Number 19 = ORIGINAL_XDG_CURRENT_DESKTOP=undefined
Environment Variable Number 20 = OS=Windows_NT
*/