#ifndef SPLIT_H_INCLUDED
#define SPLIT_H_INCLUDED

#include<string.h>
char* scanf_str(char *c)  //
{
    char t;
    for(int i=0; i<50; i++)
    {
        t=getchar();
        if(t=='\n')
        {
            c[i]='\0';
            break;
        }
        c[i]=t;
    }
    return c;
}
int get_strs(char *c, char ans[][20],int *row1)  //row¿¿¿¿
{
    /* ans[0][0]='a';
     printf("%c\n", ans[0][0]);*/
    int max_len=20;
    int row=*row1;
    /* printf("%c\n", ans[0][0]);
     printf("len is %d", strlen(c));
     printf("initial row is %d\n",row);*/
    for(int i=0; i<strlen(c); i++)
    {
        //printf("%d ",i);
        if(c[i]=='"')
        {
            ans[row][0]='"';
            i++;
            int tmp=1;

            for(; i<strlen(c)&&c[i]!='"'; i++,tmp++)
            {
                if(tmp==max_len)
                    return -1;
                ans[row][tmp]=c[i];
            }
            if(c[i]=='"')
            {
                if(tmp==max_len)
                    return -1;
                ans[row][tmp++]='"';
                ans[row][tmp]='\n';
                row++;
                tmp=0;
                i++;
                continue;
            }
            else  
            {
                return -1;  
            }
        }
        else
        {
            if(c[i]==' ')
            {
                continue;
            }
            else
            {
                int tmp=0;
                for(; i<strlen(c)&&c[i]!=' '; i++)
                {
                    ans[row][tmp]=c[i];
                    tmp++;
                }
                ans[row][tmp]='\0';
                row++;
                tmp++;
            }
        }
    }
    //printf("\n\nrow is %d\n\n", row);
    *row1=row;
    return row;

}
#endif // SPLIT_H_INCLUDED

