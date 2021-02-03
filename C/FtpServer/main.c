#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>

#define SEARCH 100
#define DOWN 200
#define END 500
#define FAIL 300
#define SUCCESS 400
#define UP 600
struct msg
{
    int type;
    int flag;
    char buf[64];
    char fname[15];
};
//将指定目录中的所有文件名发给客户端
int search_serverdir(int connfd,struct msg info)
{
    DIR *pdir;
    struct dirent *p;
    pdir = opendir("./download/");

    while((p = readdir(pdir)) != NULL)
    {
        if(p->d_name[0] == '.')
            continue;
        strcpy(info.buf,p->d_name);
        send(connfd,&info,sizeof(info),0);
    }
    info.flag = END;
    send(connfd,&info,sizeof(info),0);

}
//将某文件中的内容传给客户端
int ftp_download(int connfd,struct msg info)
{
    FILE *fp;

    char name[100];
    sprintf(name,"./download/%s",info.fname);
    fp = fopen(name,"r");
    if(fp == NULL)
    {
        info.flag = FAIL;
        send(connfd,&info,sizeof(info),0);
        return -1;
    }
    else
    {
        info.flag = SUCCESS;
        send(connfd,&info,sizeof(info),0);
    }

    int len;
    while(1)
    {
        len = fread(info.buf,1,sizeof(info.buf),fp);
        send(connfd,info.buf,len,0);

        if(len < sizeof(info.buf) && feof(fp) != 0)
            break;
    }

    fclose(fp);
    return 0;
}
//接收客户端上传的文件内容
int upload_ftp(int connfd,struct msg info)
{
    FILE *fp;
    char name[100];
    sprintf(name,"./download/%s",info.fname);

    //create and open file
    fp = fopen(name,"w");
    if(fp == NULL)
    {
        info.flag = FAIL;
        send(connfd,&info,sizeof(info),0);
        return -1;
    }
    else
    {
        info.flag = SUCCESS;
        send(connfd,&info,sizeof(info),0);
    }
    //recv file contant
    int len;
    while(1)
    {
        len = recv(connfd,info.buf,sizeof(info.buf),0);
        fwrite(info.buf,1,len,fp);
        if(len < sizeof(info.buf))
            break;
    }
    fclose(fp);



    return 0;
}

int main(int argc, const char *argv[])
{
    int sockfd;

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in seraddr;
    seraddr.sin_family = AF_INET;
   seraddr.sin_port = htons(atoi("69"));// seraddr.sin_port = htons(atoi(argv[1]));
   seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    seraddr.sin_addr.s_addr = inet_addr("192.168.199.187");

    if(bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr)) == -1)
    {
        perror("bind");
        return -1;
    }

    if(listen(sockfd,5) == -1)
    {
        perror("listen");
        return -1;
    }

    int connfd;
    struct sockaddr_in cliaddr;
    int len = sizeof(cliaddr);
    pid_t pid;
    struct msg info;
    int n;
    while(1)
    {
        connfd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);
        if(connfd == -1)
        {
            perror("accept");
            continue;
        }

        pid = fork();
        if(pid == -1)
        {
            perror("fork");
            continue;
        }
        else if(pid == 0)
        {
            close(sockfd);
            while(1)
            {
                n = recv(connfd,&info,sizeof(info),0);
                if(n == -1)
                {
                    perror("recv");
                    exit(EXIT_FAILURE);
                }
                if(n == 0)
                {
                    printf("客户端退出\n");
                    exit(EXIT_SUCCESS);
                }
                switch(info.type)
                {
                case SEARCH:
                    search_serverdir(connfd,info);
                    break;
                case DOWN:
                    ftp_download(connfd,info);
                    break;
                case UP:
                    upload_ftp(connfd,info);
                    break;
                }
            }
        }
        else
        {
            close(connfd);
        }
    }
    return 0;
}
