#ifndef WHOAMI_H_INCLUDED
#define WHOAMI_H_INCLUDED

#include<stdio.h>
#include<unistd.h>
#include<pwd.h>
char *whoami()
{
    int uid = getuid();
    printf("%s\n", getpwuid(uid)->pw_name);
    return getpwuid(uid)->pw_name;
}

#endif
