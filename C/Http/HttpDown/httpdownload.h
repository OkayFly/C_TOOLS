#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <cstdio>

#include <cstring>
#include <fcntl.h> //open
#include <unistd.h> //write
#include <cstdlib> //exit函数

#include <netdb.h>
#include <sys/stat.h>
#include <sys/time.h> //stat系统调用获取文件大小

#include <sys/socket.h>
#include <arpa/inet.h>

struct HTTP_RES_HEADER
{
    HTTP_RES_HEADER() {}
    int status_code;
    char content_type[128];
    long content_length;
};

void parse_url(const char* url, char* host, int* port, char* file_name);

HTTP_RES_HEADER parse_header(const char* response);

void get_ip_addr(char* host_name, char* ip_addr);

void progress_bar(long cur_size, long total_size, double speed);

unsigned long get_file_size(const char* filename);

void download(int client_socket, char* file_name, long content_length);

int http_download(const char* url);


#endif // HTTPDOWNLOAD_H
