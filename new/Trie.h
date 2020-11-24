#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define FALSE 0
#define TRUE 1
#ifndef MAX_LEN
#define MAX_LEN 50
#endif // MAX_LEN
typedef struct Trie{
    struct Trie* nextt[26];
    int flag;
    char ch;
    int cnt;
} Trie;
Trie* create_node(char  ch){
    Trie* node=(Trie*)malloc(sizeof(struct Trie));
    node->flag=0;
    for(int i=0;i<26;i++)
        node->nextt[i]=NULL;
    node->ch=ch;
    node->cnt=0;
    return node;
}

int check_exist(Trie *root, char* str){
    if(root==NULL||str==NULL||strlen(str)==0){
        printf("-1\n");
        return -1;
    }
    Trie *tmp=root;
    int c=str[0]-'a';
    for(int i=0;i<strlen(str);i++)
    {
       if(tmp->nextt[str[i]-'a']==NULL)
        return 0;
        tmp=tmp->nextt[str[i]-'a'];
    }
    return( tmp->cnt);
}
int check_cmd(Trie *root,char *str){
    if(root==NULL||str==NULL||strlen(str)==0){
        printf("-1\n");
        return -1;
    }
    Trie *tmp=root;
    int c=str[0]-'a';
    for(int i=0;i<strlen(str);i++)
    {
       if(tmp->nextt[str[i]-'a']==NULL)
        return 0;
        tmp=tmp->nextt[str[i]-'a'];
    }
    return( tmp->flag);
}



int input_str(Trie *root,char *str){
    if(root==NULL)
        return 0;
    Trie * tmp=root;
    for(int i=0;i<strlen(str);i++){

            if(tmp->nextt[str[i]-'a']==NULL){
                tmp->nextt[str[i]-'a']=create_node(str[i]);
            }
            tmp->cnt++;
            tmp=tmp->nextt[str[i]-'a'];
        }

    tmp->flag=1;
    int a=check_exist(root, str);
    return 1;
}

void init_tree(Trie*root)
{

    FILE *fp=NULL;;

    const char path[]="./cmd.dat";

    fp=fopen(path, "r");

    if(fp==NULL){
        printf("some wrong!");
        return ;
    }
    char buff[40];
    while(!feof(fp)){

       if(fscanf(fp,"%s",buff)==-1){
        break;
       }

       input_str(root, buff);

    }

}

Trie* get_tar_node(Trie* root, char *str){
    if(root==NULL||str==NULL||strlen(str)==0){
        printf("not found str\n");
        return  NULL;
    }
    Trie *tmp=root;
    int c=str[0]-'a';
    for(int i=0;i<strlen(str);i++)
    {

        if(tmp->nextt[str[i]-'a']==NULL)
            return NULL;
        tmp=tmp->nextt[str[i]-'a'];
    }
    return tmp;
}

void dfs(Trie* node, char *str, char *tmp,int is_singel, int depth){
    tmp[depth-1]=node->ch;
        tmp[depth]='\0';
    if(node->flag){

        //printf("ans is %s\n",tmp);
        if(is_singel){
           // printf("singel ans is %s\n",tmp);
           // strcmp(str, tmp);
            for(int i=strlen(str);i<=strlen(tmp);i++)
                str[i]=tmp[i];
        }
        else{
            printf("%s ", tmp);
        }

    }
        for(int i=0;i<26;i++){
            if(node->nextt[i]!=NULL)
            {
              //  printf("i is %d\n",i);
                dfs(node->nextt[i], str, tmp, is_singel, depth+1);
            }

        }


}
int travel(Trie * node, char* str){
    int stat=check_exist(node, str);

    if(stat==0){
      //  printf("\astat is %d", stat);

        return 0;
    }
    else
    {
        char * tmp=(char *)malloc(sizeof(char)*MAX_LEN);
        if(stat==1){
            for(int i=0;i<strlen(str);i++){
                tmp[i]=str[i];
                tmp[i+1]='\0';
            }
            dfs(get_tar_node(node, str), str, tmp, 1,strlen(str));
        }
        else{
            for(int i=0;i<strlen(str);i++){
                tmp[i]=str[i];
                tmp[i+1]='\0';
            }
            dfs(get_tar_node(node, str), str, tmp, 0,strlen(str));
        }
       free(tmp);
    }
    return stat;
}


#endif
