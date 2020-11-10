#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <string.h>
#include <fcntl.h>
#include "whoami.h"
#include "touch.h"
#define MAX_LEN 50
char str[MAX_LEN];
char record[30][MAX_LEN];
int cnt,now;
void clear_line()
{
    printf("\r");
    for(int i=0;i<50;i++)
    {
        printf(" ");
    }
    printf("\r");
}
int scanKeyboard()
{
    now=cnt;
    int t=0;
    while(1)
    {
        int in;
        struct termios new_settings;
        struct termios stored_settings;
        tcgetattr(0,&stored_settings);
        new_settings = stored_settings;
        new_settings.c_lflag &= (~ICANON);
        new_settings.c_cc[VTIME] = 0;
        tcgetattr(0,&stored_settings);
        new_settings.c_cc[VMIN] = 1;
        tcsetattr(0,TCSANOW,&new_settings);

        in = getchar();
        if(in=='\n')
        {
            str[t] = '\0';
            if(cnt>0&&strcmp(str, record[cnt-1])==0)
            {
                
            }
            else
            {
                strcpy(record[cnt], str);
                cnt++;
            }
            printf("cnt is %d\n", cnt);
            return 0;
        }
        if(in==127)
        {
            printf("\b\b\b");
        }
        else if(in=='\033')
        {
            t=0;
            in = getchar();
            if(in=='[')
            {
                in = getchar();
                if(in=='A')
                {

                clear_line();
                    if(cnt==0||now==0)
                        printf("\a");
                    else
                    {
                        strcpy(str, record[now-1]);
                        now--;
                        printf("\r%s",str );
                    }
                }
                else if(in=='B')
                {
                    
                clear_line();
                    if(cnt==0||cnt==now)
                    {
                        printf("\a");
                    }
                    else
                    {
                        strcpy(str, record[now+1]);
                        now++;
                        printf("\r%s", str);
                    }
                }
            }
        }
        else
        {
            str[t]=in;
            t++;
        }
        tcsetattr(0,TCSANOW,&stored_settings);
     
    }
   
    return 1;
}
char wd[50];
char ans[20][20];
int row=0;
int cmpStr(char *a, char *b)
{
    if(strcmp(a,b)==0)
        return 1;
    else
        return 0;
}
void func(int * flag)
{
    while(true){  
        scanKeyboard();
        row =0;
        char exit[]="exit\0";
    if(cmpStr(str,exit))
    {
        *flag=0;
        return ;
    }
    
    printf("ans[0] is %s\n", ans[0]);
    if(cmpStr(ans[0],"touch"))
    {
        touch(ans[1]);
    }
    else if(cmpStr(ans[0], "whoami"))
    {
        whoami();
    }
    else if(cmpStr(ans[0], "pwd"))
    {
        //pwd();
    }
    else if(cmpStr(ans[0], "cd"))
    {
    }
    else if(cmpStr(ans[0], "ps"))
    {

    }
    else if(cmpStr(ans[0], "wc"))
    {
        
    }
    else if(cmpStr(ans[0], "cat"))
    {
    }
    
    }
}
int get_strs()
{
    char c;
    c=getchar();
    while(c!='\n')
    {
        printf("\nread %c\n", c);
        c=getchar();
    }
}
int main()
{
    int cnt=6;
    while(1){
    scanKeyboard();
    
    }
}
