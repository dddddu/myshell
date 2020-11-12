#ifndef TOUCH_H_INCLUDED
#define TOUCH_H_INCLUDED


#include<unistd.h>
#include<fcntl.h>
#include<string.h>


// 传入的参数为一个字符串 不对字符串做任何修改
// 正常执行完毕之后会返回0 否则会返回-1 或者 -2 
int touch(char *c) 
{
	if(c==NULL)
		return -1;
	int len=strlen(c);
	if(len==0)
		return -2;
	//printf("available!\n");
        int fd=open(c, O_CREAT|O_RDWR);
        close(fd);
        //printf("create ok!\n");
	return 0;
}


#endif  //TOUCH_H_INCLUDEDE
