#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
int scanKeyboard()
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
    printf(" read %d \n" ,in);
    if(in==127)
    {
        printf("backspace\n");
    }
    if(in=='\033')
    {
        in = getchar();
        if(in=='[')
        {
            in = getchar();
            if(in=='A')
                printf("\r read top\n");
        }
       // printf("cnt char!!\n\n");
    }
    tcsetattr(0,TCSANOW,&stored_settings);
    return in;
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
    while(1)
    {
        scanKeyboard();
    }
    printf("read is !!\n");
    get_strs();
}
