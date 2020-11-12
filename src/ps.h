#ifndef PS_H_INCLUDE
#define PS_H_INCLUDE
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h> 
#include <ctype.h>
#define MAX_LEN 20
struct ps_info *trav_dir(char dir[]);
int read_info(char d_name[],struct ps_info *p1);
void print_ps(struct ps_info *head, int flag_u);
int is_num(char *);
void uid_to_name();

int print_head()
{
    struct passwd *pd;
    pd=getpwuid(getuid());
    char pwd[50];
    getcwd(pwd, sizeof pwd);
    printf("\033[36m%s@\033[34m%s \033[37m: ", pd->pw_name,pwd);
    return 0;
}
typedef struct ps_info
{
    char pname[MAX_LEN];
    char user[MAX_LEN];
    int  pid;
    int  ppid;
    char state;
    struct ps_info *next;
}mps;
int ps(int row, char av[MAX_LEN][MAX_LEN])
{
    char wd[80];
    
    int flag_u=0;
    //printf("row is %d\n\n",row);
    if(row==1)
    {

    }
    else if(row==2)
    {
        for(int i=0;i<strlen(av[1]);i++)
        {
            switch (av[1][i]){
                case 'u': flag_u=1;
             
            }
            
         }
    }
    else
    {
        exit(-1);
        printf("参数错误");
    }
    mps *head,*link;
     
    head=trav_dir("/proc/");
    if(head==NULL)
        printf("traverse dir error\n");
    print_ps(head, flag_u);
    //printf("trave ok\n"); 
    while(head!=NULL)        //释放链表
    {
        link=head;
        head=head->next;
        free(link);
    }
    chdir(wd);
    return 0;

    
}

mps *trav_dir(char dir[])                         
{
    DIR *dir_ptr;
    mps *head,*p1,*p2;
    struct dirent *direntp;
    struct stat infobuf;
 
    if((dir_ptr=opendir(dir))==NULL)
        fprintf(stderr,"dir error %s\n",dir);
    else
    {
        head=p1=p2=(struct ps_info *)malloc(sizeof(struct ps_info));    //建立链表
        while((direntp=readdir(dir_ptr)) != NULL)               //遍历/proc目录所有进程目录
        {
            if((is_num(direntp->d_name))==0)                   //判断目录名字是否为纯数字
            {
                if(p1==NULL)
                {
                    printf("malloc error!\n");
                    exit(0);
                }
 
                if(read_info(direntp->d_name,p1)!=0)         //获取进程信息
                {
                    printf("read_info error\n");
                    exit(0);
                }
                p2->next=p1;                        //插入新节点
                p2=p1;
                p1=(struct ps_info *)malloc(sizeof(struct ps_info));
            }
        }
    }
    p2->next=NULL;
    return head;
}
 
int read_info(char d_name[],struct ps_info *p1)
{
    FILE *fd;
    char dir[20];
    struct stat infobuf;
 
    sprintf(dir,"%s/%s","/proc/",d_name);
    chdir("/proc");                        //切换至/proc目录，不然stat返回-1
    if(stat(d_name,&infobuf)==-1)                     //get process USER
        fprintf(stderr,"stat error %s\n",d_name);
    else
        //p1->user=uid_to_name(infobuf.st_uid);
        uid_to_name(infobuf.st_uid, p1);
 
    chdir(dir);                         //切换到/proc/pid目录
    if((fd=fopen("stat","r"))<0)
    {
        printf("open the file is error!\n");
        exit(0);       
    }
    while(4==fscanf(fd,"%d %s %c %d\n",&(p1->pid),p1->pname,&(p1->state),&(p1->ppid)))     //读文件的前四个字段内容，并存放在相关的链表成员中
    {
        break;                                    
    }
    fclose(fd);
    return 0;
}
 
void uid_to_name(uid_t uid, struct ps_info *p1)         //由进程uid得到进程的所有者user
{
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];
 
    if((pw_ptr=getpwuid(uid)) == NULL)
    {
        sprintf(numstr,"%d",uid);
        strcpy(p1->user, numstr);
    }
    else
        strcpy(p1->user, pw_ptr->pw_name);
}
 
int is_num(char str[])
{
    int i,len;
    len=strlen(str);
    if(len==0) return -1;
    for(i=0;i<len;i++)
        if(!isdigit(str[i]))
            return -1;
    return 0;
}
 
void print_ps(struct ps_info *head, int flag_u)
{
    printf("flag is %d\n\n", flag_u);
    mps *list;
    char *user_name;
    user_name=getpwuid(getuid())->pw_name;
    printf("USER\t\tPID\tPPID\tSTATE\tPNAME\n");
    for(list=head;list!=NULL;list=list->next)
    {
        if(flag_u)
        {
            if(strcmp(list->user, user_name)!=0)
                continue;
        }
        printf("%s\t\t%d\t%d\t%c\t%s\n",list->user,list->pid,list->ppid,list->state,list->pname);
    }
}
#endif
