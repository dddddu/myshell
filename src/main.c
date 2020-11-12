#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <string.h>
#include <fcntl.h>
#include "whoami.h"
#include "touch.h"
#include "split.h"
#include "ps.h"
#include "fop.h"
#include "all.h"
#include "wc.h"
#include "mv.h"
#include "cp.h"
#define MAX_LEN 50
char str[MAX_LEN];
char record[30][MAX_LEN];
char ans[20][20];
int cnt,now;
void clear_line()
{
    printf("\r");
    for(int i=0;i<60;i++)
    {
        printf(" ");
    }
    printf("\r");
}
int insert_char(char *source, int index,char c)
{

    if(source==NULL||index>=strlen(source))
    {
        return -1;
    }
    int len=strlen(source);
    source[len+1]='\0';
    for(int i=len;i>=index;i--)
    {
        source[i+1]=source[i];
    }
    source[index]=c;
    return 0;
}

int delete_char(char *source, int index)
{

    if(source==NULL||index>=strlen(source)||strlen(source)==0)
    {
        return -1;
    }

    int len=strlen(source);
    for(int i=index+1;i<len;i++)
    {
        source[i-1]=source[i];
    }
    source[len-1]='\0';
    //printf("after delete is %s ", source);
    return 0;
}



int scanKeyboard()
{
    now=cnt;
    int t=0;
    int len=0;
    int cursor_place=0;
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
            str[len] = '\0';
            if(cnt>0&&strcmp(str, record[cnt-1])==0)
            {
            }
            else
            {
                strcpy(record[cnt], str);
                cnt++;
            }
           /* printf("str is %s\n", str);
            printf("len is %d\n", strlen(str));
            printf("cnt is %d\n", cnt);*/
        
            return 0;
        }
        if(in==127)
        {
        
       
            clear_line();
            if(cursor_place==0)
            {
                print_head();
                continue;
            }
            delete_char(str, cursor_place-1);
            len--;
            cursor_place--;
            if(len==cursor_place)
            {
                print_head();
                printf("%s", str);
                continue;
            }
            print_head();
            printf("%s\033[%dD",str,len-cursor_place+1);
        
        }
        else if(in=='\033')
        {
            
            in = getchar();
            if(in=='[')
            {
                in = getchar();
                if(in=='A')
                {

                    if(cnt==0||now==0)
                    {
                        printf("\a");
                        clear_line();
                        print_head();
                    }
                    else
                    {
                        now--;
                        strcpy(str, record[now]);
                        len = strlen(record[now]);
                        cursor_place=len;
                        clear_line();
                        print_head();
                        printf("%s",str );
                    }
                }
                else if(in=='B')
                {
                    
                clear_line();
                    if(cnt==0||cnt==now)
                    {
                        
                        printf("\a");
                        if(cnt==0)
                            print_head();
                        else
                        {
                            print_head();
                            printf("%s",str);
                        }
                    }
                    else
                    {
                         now++;
                        
                        print_head();
                        len=strlen(record[now]);
                        cursor_place=len;
                        
                        strcpy(str, record[now]);
                        printf("%s", str);
                    }
                }
                else if(in=='C')
                {
                    clear_line();
                    print_head();
                    if(len==cursor_place)
                    {
                        printf("%s", str);    
                        continue;
                    }
                    else
                    {
                    
                       // printf("%d ",len-cursor_place);
                       cursor_place++;
                        printf("%s\033[%dD", str, len-cursor_place);
                    }
                    //printf("\b\b\b\033[3C");
                }
                else if(in=='D')
                {
                    //printf("hello");
                    clear_line();
                    print_head();
                    if(cursor_place==0)
                    {
                        printf("%s\033[1D",str);
                        continue;
                    }
                    else
                    {
                        int dif=len-cursor_place+1;
                        cursor_place--;
                        printf("%s\033[%dD", str, dif);
                    }
                    
                }
            }
        }
        else
        {
            if(len!=cursor_place)
            {
                clear_line();
                print_head();
                int dif = len-cursor_place+1;
               insert_char(str,cursor_place, in);
               len++;
               cursor_place++;
                printf("%s\033[%dD", str, dif);
            }else
            {
                str[len]=in;
                str[len+1]='\0';
                len++,cursor_place++;
            }
        
        }
        tcsetattr(0,TCSANOW,&stored_settings);
     
    }
   
    return 1;
}
int cmpStr(char *a, char *b)
{
    if(strcmp(a,b)==0)
        return 1;
    else
        return 0;
}

int deal_with_str(char *str)
{
    int len= strlen(str);
    while(len>0&&str[len-1]==' ')
        str[len-1]='\0';
}
void func()
{
    int row=0;
    while(1){ 
        print_head();
        row =0;
        scanKeyboard();
        deal_with_str(str);
        get_strs(str, ans, &row);
        if(cmpStr(str,"exit"))
        {
            return ;
        }
    
        if(cmpStr(ans[0],"touch"))
        {
            touch(ans[1]);
        }
        else if(cmpStr(ans[0], "whoami"))
        {
            whoami();
        }
        else if(cmpStr(ans[0], "clear"))
        {
            printf("\33[H\033[2J\n");
        }
        else if(cmpStr(ans[0], "pwd"))
        {
        //pwd();
            pwd_main();
        }
        else if(cmpStr(ans[0], "cd"))
        {
            cd(ans[1]);
        }
        else if(cmpStr(ans[0], "ps"))
        {
            char now_dir[50];
            getcwd(now_dir, sizeof now_dir);
            ps(row, ans);
            cd(now_dir);
        }
        else if(cmpStr(ans[0], "cat"))
        {
            if(row<2)
                continue;
            cat_main(row, ans);     
        }
        else if(cmpStr(ans[0], "rm"))
        {
            if(row<=1)
                continue;
            rm(ans[1]);
        }
        else if(cmpStr(ans[0], "ls"))
        {
            ls_main(row, ans);
        }
        else if(cmpStr(ans[0], "echo"))
        {
            echo_main(row, ans);
        }
        else if(cmpStr(ans[0], "mv"))
        {
            if(row<3)
            {
                printf("wrong para!\n");
                return -1;
            }
            mv(row, ans);
        }
        else if(cmpStr(ans[0], "wc"))
        {
            if(row<2)
            {
              printf("wrong para!\n");
              return -1;
            }
            wc(ans[1]);
            
        }
        else if(cmpStr(ans[0], "cp"))
        {
            if(row<3)
            {
                printf("wrong para!\n");
                return -1;
            }
            cp(row, ans);
        }
        else
        {
            printf("wrong commond!\n");
        }
    }

}
int main()
{
    func();
}
