#ifndef __FTP_DOWN_H__
#define __FTP_DOWN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
// #include <curses.h>
// #include <termios.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>



int ftp_download(int uart_num, char* filename);
int xconnect(struct sockaddr_in *s_addr, int type);

int create_data_sock();
int create_port_data_sock();
int create_pasv_data_sock();

int send_command(char *cmd, char *buf, int sockfd);
int ftp_get_reply(int sock_fd);
int get_port();

//send command to server with sock_fd
int ftp_send_cmd(const char *s1, const char *s2, int sock_fd);

int rand_local_port();
int set_bin_mode(int mode, int sockfd);

int get_port_data_connection(int sockfd);

//connect data stream
int xconnect_ftpdata();
int file_copy(int srcfd, int uart_num);
int ftp_login();

//call this function to quit
void ftp_quit();
int ftp_init(char *hostip, int port);
int do_firmware_upgrade_ftp(char *url, uint32_t total_length, uint32_t crc32);


#endif