#ifndef ALL_H_INCLUDE
#define ALL_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include <stdlib.h>

int cat_main(int argc, char argv[][20])
{
	FILE *src_fp;
	int read_ret;
	
	if(argc<2)
	{
		printf("please input src file\n");
		return -1;
	}
	src_fp=fopen(argv[1],"r");

	if(src_fp==NULL)
	{
		printf("open src_file %s failure\n",argv[1]);
		return -2;
	}
//	printf("open src_file %s success\n",argv[1]);
	

	while(1)
	{
		read_ret=fgetc(src_fp);
		if(feof(src_fp))
		{
			//printf("read file %s end\n",argv[1]);
			break;
		}
		fputc(read_ret,stdout);
	}
	fclose(src_fp);
	return 0;
}

int echo_main(int argc, char argv[][20])
{
    int i;
    for (i = 1; i < argc; i++)
        printf("%s\n", argv[i]);
    printf("\n");
    return 0;
}

void pwd_main()
{
    char buf[1024];
    char *cwd = getcwd(buf, sizeof(buf));

    if (cwd==NULL)
    {
        perror("Get cerrent working directory fail.\n");
        exit(-1);
    }
    else
    {
        printf("%s\n", cwd);
    }
}

#define PARAM_NONE 0 
#define PARAM_A 1    
#define PARAM_L 2    
#define PARAM_R 4   
#define PARAM_r 8
#define MAXROWLEN 80

char PATH[PATH_MAX + 1]; 
int flag;                

int g_leave_len = MAXROWLEN;
int g_maxlen;

void my_err(const char *err_string, int line); 
void display_dir(char *path);            

void my_err(const char *err_string, int line)
{
    
    fprintf(stderr, "line:%d", __LINE__);
    
    perror(err_string);
    exit(1);
}

void cprint(char *name, mode_t st_mode) 
{
    
    if (S_ISLNK(st_mode))                                   
        printf("\033[1;36m%-*s  \033[0m", g_maxlen, name);     
    else if (S_ISDIR(st_mode) && (st_mode & 000777) == 0777) 
        printf("\033[1;34;42m%-*s  \033[0m", g_maxlen, name);
    else if (S_ISDIR(st_mode)) 
        printf("\033[1;34m%-*s  \033[0m", g_maxlen, name);
    else if (st_mode & S_IXUSR || st_mode & S_IXGRP || st_mode & S_IXOTH) 
        printf("\033[1;32m%-*s  \033[0m", g_maxlen, name);
    else 
        printf("%-*s ", g_maxlen, name);
}

void display_attribute(char *name)
{
    struct stat buf; 
    char buff_time[32];
    struct passwd *psd;
    struct group *grp;
    if (lstat(name, &buf) == -1)
    {
        my_err("stat", __LINE__);
    }

    
    if (S_ISLNK(buf.st_mode))
        printf("l");
    else if (S_ISREG(buf.st_mode))
        printf("-");
    else if (S_ISDIR(buf.st_mode))
        printf("d");
    else if (S_ISCHR(buf.st_mode))
        printf("c");
    else if (S_ISBLK(buf.st_mode))
        printf("b");
    else if (S_ISFIFO(buf.st_mode))
        printf("f");
    else if (S_ISSOCK(buf.st_mode))
        printf("s");    
    if (buf.st_mode & S_IRUSR)
        printf("r");
    else
        printf("-");
    if (buf.st_mode & S_IWUSR)
        printf("w");
    else
        printf("-");
    if (buf.st_mode & S_IXUSR)
        printf("x");
    else
        printf("-");

    
    if (buf.st_mode & S_IRGRP)
        printf("r");
    else
        printf("-");
    if (buf.st_mode & S_IWGRP)
        printf("w");
    else
        printf("-");
    if (buf.st_mode & S_IXGRP)
        printf("x");
    else
        printf("-");

    
    if (buf.st_mode & S_IROTH)
        printf("r");
    else
        printf("-");
    if (buf.st_mode & S_IWOTH)
        printf("w");
    else
        printf("-");
    if (buf.st_mode & S_IXOTH)
        printf("x");
    else
        printf("-");

    printf(" ");
    
    psd = getpwuid(buf.st_uid);    
    grp = getgrgid(buf.st_gid);    
    printf("%4ld ", buf.st_nlink); 
    printf("%-8s ", psd->pw_name); 
    printf("%-8s ", grp->gr_name); 
    printf("%6ld ", buf.st_size);   

    strcpy(buff_time, ctime(&buf.st_mtime)); 
    

    buff_time[strlen(buff_time) - 1] = '\0'; 
    printf(" %s ", buff_time);             
    cprint(name, buf.st_mode);               
    printf("\n");                            
}

void displayR_attribute(char *name) 

{
    struct stat buf;

    if (lstat(name, &buf) == -1)
    {
        my_err("stat", __LINE__);
    }
    if (S_ISDIR(buf.st_mode))
    {
        display_dir(name);
        free(name); 
        char *p = PATH;
        while (*++p);
        while (*--p != '/');
        *p = '\0';   
        chdir(".."); 
        return;
    }
}
void display_single(char *name) 
{
    int len;
    struct stat buf;
    if (lstat(name, &buf) == -1) 
    {
        return;
    }

    if (g_leave_len < g_maxlen)
    {
        printf("\n");
        g_leave_len = MAXROWLEN;
    }

    cprint(name, buf.st_mode); 
    g_leave_len = g_leave_len - (g_maxlen + 2);
}

void displayR_single(char *name) 
{
    int len;
    struct stat buf;
    if (lstat(name, &buf) == -1)
    {
        return;
    }
    if (S_ISDIR(buf.st_mode))
    {
        printf("\n");

        g_leave_len = MAXROWLEN;

        display_dir(name);
        free(name); 
        char *p = PATH;
        while (*++p)
            ;
        while (*--p != '/')
            ;
        *p = '\0';
        chdir(".."); 
    }
}

void display(char **name, int count) 
{
    switch (flag)
    {
        int i;
    case PARAM_r:    
    case PARAM_NONE: 
        for (i = 0; i < count; i++)
        {
            if (name[i][0] != '.') 
                display_single(name[i]);
        }
        break;
    case PARAM_r + PARAM_A: 
    case PARAM_A:           
        for (i = 0; i < count; i++)
        {
            display_single(name[i]);
        }
        break;
    case PARAM_r + PARAM_L: 
    case PARAM_L:           
        for (i = 0; i < count; i++)
        {
            if (name[i][0] != '.')
            {
                display_attribute(name[i]);
            }
        }
        break;
    case PARAM_R + PARAM_r: 
    case PARAM_R:           
        
        for (i = 0; i < count; i++)
        {
            if (name[i][0] != '.')
            {
                display_single(name[i]);
            }
        }
        
        for (i = 0; i < count; i++)
        {
            if (name[i][0] != '.') 
            {
                displayR_single(name[i]);
            }
        }
        break;
    case PARAM_L + PARAM_r + PARAM_R:
    case PARAM_R + PARAM_L: 
        for (i = 0; i < count; i++)
        {
            if (name[i][0] != '.')
            {
                display_attribute(name[i]);
            }
        }
        for (i = 0; i < count; i++)
        {
            if (name[i][0] != '.')
            {
                displayR_attribute(name[i]);
            }
        }
        break;
    case PARAM_A + PARAM_r + PARAM_R:
    case PARAM_R + PARAM_A:
        for (i = 0; i < count; i++)
        {
            display_single(name[i]);
        }
        for (i = 0; i < count; i++)
        {
            if (name[i][0] != '.')
            {
                displayR_single(name[i]);
            }
        }
        break;

    case PARAM_A + PARAM_L + PARAM_r:
    case PARAM_A + PARAM_L:
        for (i = 0; i < count; i++)
        {
            display_attribute(name[i]);
        }
        break;
    case PARAM_A + PARAM_L + PARAM_R + PARAM_r:
    case PARAM_A + PARAM_L + PARAM_R:
        for (i = 0; i < count; i++)
        {
            display_attribute(name[i]);
        }
        for (i = 0; i < count; i++)
        {
            if (name[i][0] != '.')
            {
                displayR_attribute(name[i]);
            }
        }
        break;

    default:
        break;
    }
}

void display_dir(char *path) 
{
    DIR *dir;           
    struct dirent *ptr; 
    int count = 0;      
    
    if ((flag & PARAM_R) != 0) 
    {
        int len = strlen(PATH); 
        if (len > 0)
        {
            if (PATH[len - 1] == '/')
                PATH[len - 1] = '\0'; 
        }
        if (path[0] == '.' || path[0] == '/')
        {
            strcat(PATH, path); 
        }
        else
        {
            strcat(PATH, "/");
            strcat(PATH, path);
        }

        printf("%s:\n", PATH); 
    }

    
    dir = opendir(path);
    if (dir == NULL)
        my_err("opendir", __LINE__);
    g_maxlen = 0;

    while ((ptr = readdir(dir)) != NULL)
    {
        if (g_maxlen < strlen(ptr->d_name))
            g_maxlen = strlen(ptr->d_name); 
        count++;                            
    }

    closedir(dir);

    char **filenames = (char **)malloc(sizeof(char *) * count), temp[PATH_MAX + 1]; 
    for (int i = 0; i < count; i++)                                                 
    {                                                                               
        filenames[i] = (char *)malloc(sizeof(char) * PATH_MAX + 1);
    }

    int i, j;
    
    dir = opendir(path);
    for (i = 0; i < count; i++)
    {
        ptr = readdir(dir);
        if (ptr == NULL)
        {
            my_err("readdir", __LINE__);
        }
        strcpy(filenames[i], ptr->d_name); 
    }
    closedir(dir);

    
    if (flag & PARAM_r) 
    {
        for (i = 0; i < count - 1; i++)
        {
            for (j = 0; j < count - 1 - i; j++)
            {
                if (strcmp(filenames[j], filenames[j + 1]) < 0)
                {
                    strcpy(temp, filenames[j]);
                    strcpy(filenames[j], filenames[j + 1]);
                    strcpy(filenames[j + 1], temp);
                }
            }
        }
    }
    else 
    {
        for (i = 0; i < count - 1; i++)
        {
            for (j = 0; j < count - 1 - i; j++)
            {
                if (strcmp(filenames[j], filenames[j + 1]) > 0)
                {
                    strcpy(temp, filenames[j]);
                    strcpy(filenames[j], filenames[j + 1]);
                    strcpy(filenames[j + 1], temp);
                }
            }
        }
    }

    if (chdir(path) < 0)
    {
        my_err("chdir", __LINE__);
    }

    display(filenames, count);

}

int ls_main(int argc, char argv[][20])
{
    int i, j, k = 0, num = 0; 

    char param[32] = "";                                     
    char *path[1];                                           
    path[0] = (char *)malloc(sizeof(char) * (PATH_MAX + 1)); 
    
    flag = PARAM_NONE; 
    struct stat buf;   
    
    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (j = 1; j < strlen(argv[i]); j++)
            {
                param[k] = argv[i][j];
                k++;
            }
            num++;
        }
    }
    param[k] = '\0';

    for (i = 0; i < k; i++)
    {
        if (param[i] == 'a')
            flag |= PARAM_A;
        
        else if (param[i] == 'l')
            flag |= PARAM_L;
        else if (param[i] == 'R')
            flag |= PARAM_R;
        else if (param[i] == 'r')
            flag |= PARAM_r;
        else
        {
            
            printf("my_ls:invalid option -%c\n", param[i]);
            exit(0); 
        }
    }
    
    if (num + 1 == argc)
    {
        strcpy(path[0], "."); 
        display_dir(path[0]); 
		printf("\n");
        return 0;
    }

    i = 1;
    do
    {
        if (argv[i][0] == '-')
        {
            i++;
            continue;
        }
        else
        {
            strcpy(path[0], argv[i]);
            
            if (stat(argv[i], &buf) == -1)
            {
                my_err("stat", __LINE__); 
            }
            if (S_ISDIR(buf.st_mode)) 
            {
                display_dir(path[0]);
                i++;
            }
            else
            {
                
                display(path, 1);
                i++;
            }
        }
    } while (i < argc);
    printf("\n");

	return 0;

}

#endif
