#ifndef CP_H_INCLUDED
#define SPLIT_H_INCLUDED
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>

int cp(int argc, char argv[][20])
{
	int fd,fd1,size;
	int readnumber,writenumber;
	struct stat statbuf,statbuf1; 
	char tempor[1024];

	fd = open(argv[1],O_RDONLY,777);
	if(fd < 0)
	{
		perror("open wrong!");
		return 0;
	}

	stat(argv[1],&statbuf);
	size = statbuf.st_size;
	readnumber = read(fd,tempor,size);
	printf("you have read %d words \n",readnumber);

    stat(argv[2],&statbuf1);
   
    if(S_ISDIR(statbuf1.st_mode) != 0)
    {
   
    	if(S_ISDIR(statbuf.st_mode))
    	{
	    	char ch = '/';
	    	char *q = strchr(argv[1],ch);
	    	printf("%s\n", q);
	    	strcat(argv[2],q);

    	}else 
    	{
    		char *p = "/";
    		strcat(argv[2],p);
    		strcat(argv[2],argv[1]);
    	 }
    }
	chmod(argv[2],S_IRUSR|S_IWUSR);
	printf("%s\n",argv[2] );
	fd1 = open(argv[2],O_WRONLY|O_CREAT,777);

	if(fd1 < 0)
	{
		perror("open wrong!");
		return 0;
	}
 
	writenumber = write(fd1,tempor,readnumber);
	printf("you have write %d words \n",writenumber);


	close(fd); 
	close(fd1);


	return 0;
}
#endif
