
/*
    !!!!读入命令最长不能超过50个
*/


#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
void scanf_str(char *c)
{
    char t;
   for(int i=0;i<50;i++)
   {
        t=getchar();
        if(t=='\n')
        {
            c[i]='\0';
            break;
        }
        c[i]=t;
   }
}

/*
c是一个字符串 要分割的字符串， 它由scanf_str()函数读取而来
ans[][20]  是分割之后保存的位置   20代表分割之后每行只能有19个字符， 最后要以'\0'结尾
row 记录分割之后ans的行数， 传入一个int类型是数据 由于c由于没有引用 ，所以使用的时候
要以传地址的方式传入

最后返回的是行数 如果小于0代表引号没有结尾
如果等于零代表传入的是空值
处理的结果存储在ans里面
*/
int get_ans(char *c, char ans[][20],int *row1)  //row代表行数
{

   /* ans[0][0]='a';
    printf("%c\n", ans[0][0]);*/
    int row=*row1;
    printf("%c\n", ans[0][0]);
    printf("len is %d", strlen(c));
    printf("initial row is %d\n",row);
    bool flag=false;
    for(int i=0;i<strlen(c);i++)
    {
        //printf("%d ",i);
        if(c[i]=='"')
        {
            ans[row][0]='"';
            i++;
            int tmp=1;
            if(flag==false)   //加这行有点多余了 本来向加一个标志位判断是不是 "结尾
            {
                for(;i<strlen(c)&&c[i]!='"';i++,tmp++)
                {
                     ans[row][tmp]=c[i];
                }
                if(c[i]=='"')
                {
                    ans[row][tmp++]='"';
                    ans[row][tmp]='\n';
                    row++;
                    tmp=0;
                    i++;continue;
                }
                else
                {
                    ans[row][++tmp]='\n';
                    row++;
                    tmp=0;
                    printf("input error!\n");
                    return -1;   //没有匹配到 " 代表错误
                }
            }
        }
        else{
            if(c[i]==' ')
            {
                continue;
            }
            else
            {
                //printf("a ");
                int tmp=0;
                for(;i<strlen(c)&&c[i]!=' ';i++)
                {
                    //printf("here i is %c  row is %d tmp is %d", c[i],row,tmp);
                    ans[row][tmp]=c[i];
                    tmp++;
                   // printf("\nafter is is%c  ", ans[row][tmp-1]);
                }
                ans[row][tmp]='\0';
              //  printf("%s\n", ans[row]);
                row++;
                tmp++;
            }
        }


    }
    printf("\n\nrow is %d\n\n", row);
    *row1=row;
    return row;

}

int main()
{
    char c[50];
    char ans[20][20];
    int row=0;
    char t;
    ans[0][0]='a';
    //printf("%c", ans[0][0]);
    scanf_str(c);
    get_ans(c,ans,&row);
    //printf("%d ", row);
    printf("\n\n\n");
    for(int i=0;i<row;i++)
    {
        printf("num %d %s\n",i, ans[i]);
    }
}
