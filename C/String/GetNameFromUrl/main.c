#include <stdio.h>
#include "string.h"


 void get_file_name_from_url1(const char* url, char* ftp_filename)
{
    memcpy(ftp_filename, strrchr(url,'/')+1,strlen(strrchr(url,'/')-1));
}
static void add_char(char* buf, char ch)
{
    char a[2] = {0};
    a[0] = ch;
    strcat(buf,a);
}

char ret_val[256] = {0};
static char* get_file_Name_from_url(char *url)
{
    char  buf[256] = {0};
    //inverse file name
    for(int i=strlen(url); *(url+i)!='/'; i--)
    {
        add_char(buf, *(url+i));
    }

    //over turn the name
    for( int i=strlen(buf)-1, j=0; *(buf+i)!=0; i--, j++)
    {
        ret_val[j] = *(buf+i);
    }
    return ret_val;
}

int main()
{
    printf("get_file_Name_from_url!\n");
    char* url = "http://47.105.217.224:9002/ota/35fbee64-f3e0-49a1-9858-3c8e58100cf9.bin";
    char* file_name = get_file_Name_from_url(url);
    printf("@@ url:%s\n", url);
    printf("@@filename:%s\n", file_name);
    char name[100];
    get_file_name_from_url1(url, name);
    printf("@@name:%s\n",name);

    return 0;
}
