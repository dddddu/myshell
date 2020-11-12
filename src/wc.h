#ifndef WC_H_INCLUDED
#define SPLIT_H_INCLUDED

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/wait.h>

void wc(char argv[20])
{
    
    int pid;
    int fd,nbytes;
    int flags=O_CREAT|O_RDONLY;
    char buf[BUFSIZ];
    char buff[BUFSIZ];
    size_t rcount,wcount;
    int p_fd[2];
    size_t n;
    //创建管道
    if(pipe(p_fd)<0)
    {
            //返值小于0，创建失败
            fprintf(stderr,"%s:pipe failed:%s\n",argv,strerror(errno));
    }


    //创建子进程
    if((pid=fork())<0)
    {
	    fprintf(stderr,"%s:fork of child failed:%s\n",argv,strerror(errno));
	    exit(1);
    }
  
    else if (pid==0)
    {
            //打开文件
            if((fd=open(argv,flags,0666))<0)
	    {
                perror("Parameter ERROR!");
                exit(EXIT_FAILURE);
            }
            while((nbytes=read(fd,buf,BUFSIZ))!=0)
	    {
                  n=strlen(buf);
                  //向管道写数据
                  if((wcount=write(p_fd[1],buf,n))!=n)
		  {
                        fprintf(stderr,"%s:write failed:%s\n",argv,strerror(errno));
                        exit(0);
                  }
            }
            close(fd);
            exit(EXIT_SUCCESS);

    }else
    {
             //父进程
             wait(0);//等待子进结束
             int len=0;
             //从管道读数据,p_fd[0]为读
             if((rcount=read(p_fd[0],buff,BUFSIZ))==0)
	     {
                 fprintf(stderr,"%s:read failed:%s\n",argv,strerror(errno));
                 exit(0);
             }
             buff[rcount]='\0';
             len=strlen(buff);
             printf("%d\n",len);
    }
    close(p_fd[0]);
    close(p_fd[1]);
}

#endif
