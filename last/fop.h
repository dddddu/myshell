#ifndef FOP_H_INCLUDED
#define FOP_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
int get_access(char *f)  //filename
{
    int mode=0;
    if(access(f, F_OK)==-1)
            return -1;
    if(access(f, W_OK)!=-1)
            mode+=2;
   if(access(f,R_OK)!=-1)
            mode+=4;
    if(access(f,X_OK)!=-1)
            mode+=1;
    return mode;
}
int rm(char *dir) //dirname
{  
    if(access(dir, F_OK)==-1)
    {
            printf("%s\n", dir);
            char str[]="file not exists!\n";
            write(1, str, strlen(str));
            return -1;
    }
    char cur_dir[] = ".";
    char up_dir[] = "..";
    char dir_name[128];
    DIR * dirp;
    struct dirent *dp;
    struct stat dir_stat;
   
    if(0>stat(dir, &dir_stat)){
        return -1;
    }
    if(S_ISREG(dir_stat.st_mode)){
        remove(dir);
    }
    else if(S_ISDIR(dir_stat.st_mode)){
        dirp = opendir(dir);
        while((dp=readdir(dirp))!=NULL){
            if(0==(strcmp(cur_dir, dp->d_name))||(0==strcmp(up_dir, dp->d_name))){
            continue;
            }
            sprintf(dir_name, "%s//%s", dir, dp->d_name);
            rm(dir_name);
        }
        closedir(dirp);
        rmdir(dir);
    }else{
        char info[]="unknown file type! \n";
        write(1, info, strlen(info));
        return -1;
    }
    return 0;
}
int cd(char *str)
{
    if(access(str, F_OK)==-1)
    {
        char to_print[]="file don't exist!\n";
        write(2,to_print, sizeof to_print);
        return -1;
    }
    else
    {
        struct stat buf;
        lstat(str, &buf);
        if(S_ISREG(buf.st_mode))
        {
            char to_print[]="is not dir!\n";
            write(2, to_print, sizeof to_print);
            return -1;
        }
        else
        {
            chdir(str);
            return 0;
        }
    }
}
#endif
