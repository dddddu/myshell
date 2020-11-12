#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <string.h>
#include <fcntl.h>
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

    if(source==NULL||index>=strlen(source)||index==0)
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
            printf("str is %s\n", str);
            printf("len is %d\n", strlen(str));
            printf("cnt is %d\n", cnt);
            return 0;
        }
        if(in==127)
        {

            clear_line();
            delete_char(str, cursor_place-1);
            len--;
            cursor_place--;
            printf("\r%s\033[%dD",str,len-cursor_place);
        }
        else if(in=='\033')
        {
            
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
                        now--;
                        strcpy(str, record[now]);
                        len = strlen(record[now]);
                        cursor_place=len;
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
                         now++;
                        strcpy(str, record[now]);
                        len=strlen(record[now]);
                        cursor_place=len;
                        printf("\r%s", str);
                    }
                }
                else if(in=='C')
                {
                    clear_line();
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
                int dif = len-cursor_place;
               insert_char(str,cursor_place, in);
               len++;
               cursor_place++;
                printf("\r%s\033[%dD", str, dif);
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
int main()
{
    int cnt=6;
    while(1){
    scanKeyboard();
    
    }
}
