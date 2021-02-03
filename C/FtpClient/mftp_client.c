#include "mftp_client.h"

#include <string.h>
#include <stdlib.h>
#include "uart_crc.h"

#define DEFAULT_FTP_PORT 21
#define BUFSIZE 1024

static uint8_t kIsFtpInit = 0;
// #define DEFAULT_TFTP_HOST "192.168.1.210"
//#define FILE_NAME "4890d4cb-d245-4008-bf99-e5e5a842adbc.bin"

struct hostent *server_hostent;
struct sockaddr_in ftp_server, local_host;

static const char user[10] = "zx";   //ftp usr
static const char passwd[20] = "HXftp123456"; //ftp passwd
char buffer[1024*1000] = {'0'};
static int sock_control;
static const int mode = 0; //ftp mode , 0 is PORT, 1 is PASV; 
//1 err connect: Connection timed out...error connect...Error create_data_sock

static const char *TAG = "FTP_DOWNLOAD";
// extern void notify_reset(int port);

int fill_host_addr(char *host_ip_addr, struct sockaddr_in *host, int port)
{
    if (port <= 0 || port > 65535)
        return 254;
    // bzero(host, sizeof(struct sockaddr_in));
    memset(host, 0, sizeof(struct sockaddr_in));
    host->sin_family = AF_INET;
    if (inet_addr(host_ip_addr) != -1)
    {
        host->sin_addr.s_addr = inet_addr(host_ip_addr);
    }
    else
    {
        if ((server_hostent = gethostbyname(host_ip_addr)) != 0)
        {
            memcpy(&host->sin_addr, server_hostent->h_addr, sizeof(host->sin_addr));
        }
        else
            return 253;
    }

    host->sin_port = htons(port);
    return 1;
}

int xconnect(struct sockaddr_in *s_addr, int type)
{
    struct timeval out_time;
    int set;
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        printf("...creat socket errr!");
        return -1;
    }

    if (type == 1)
    {
        out_time.tv_sec = 0;
        out_time.tv_usec = 300 * 1000;
    }
    else
    {
        out_time.tv_sec = 5;
        out_time.tv_usec = 0;
    }

    set = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &out_time, sizeof(out_time));
    if (set != 0)
    {
        printf("...set socket %s errno:%d\n", strerror(errno), errno);
        return -1;
    }
    //connect to the server
    if (connect(s, (struct sockaddr *)s_addr, sizeof(struct sockaddr_in)) < 0)
    {
        printf("Can't connect to server %s, port %d\n",
               inet_ntoa(s_addr->sin_addr), ntohs(s_addr->sin_port));
        return -1;
    }
    return s;
}

int send_command(char *cmd, char *buf, int sockfd)
{
    int read_size = 0;
    int write_size = 0;
    int need_free_buf = 0;
    int ret_value = -1;
    if (sockfd == -1)
    {
        return (-1);
    }
    if (!buf)
    {
        buf = (char *)malloc(sizeof(char) * BUFSIZE);
        need_free_buf = 1;
    }
    if (cmd)
    {
        //		printf("command: %s", cmd);
        write_size = write(sockfd, cmd, strlen(cmd));
    }
    //	printf("send_command: write_size: %d\n", write_size);
    read_size = read(sockfd, buf, sizeof(char) * BUFSIZE);
    //	printf("send_command: read_size: %d\n", read_size);
    if (read_size > 0)
    {
        buf[read_size] = '\0';
        printf("%s", buf);
        ret_value = atoi(buf);
    }
    if (need_free_buf)
        free(buf);
    return (ret_value);
}

int create_data_sock()
{
    if (mode)
    {
        return create_pasv_data_sock();
    }
    else
    {
        return create_port_data_sock();
    }
}

int create_pasv_data_sock()
{
    int ret_value = 0;
    char *buf;
    char *pos_start;
    char *pos_end;
    char *pos_cur;
    char *part[6];
    char dest_ip[20];
    int data_port;
    int i = 0;
    int sockfd = -1;
    struct sockaddr_in server_addr;
    struct hostent *host_ent;
    buf = (char *)malloc(sizeof(char) * BUFSIZE);
    //	printf("sock_control: %d\n", sock_control);
    if ((ret_value = send_command("PASV\r\n", buf, sock_control)) != 227)
    {
        fprintf(stderr, "connection is closed by remote server\n");
        printf("...connection is closed by remote server\n");
        //        exit(1);
        kIsFtpInit = 0;
        return -1;
    }
    
    printf("...send_command successful: %d\n", ret_value);
    pos_start = strchr(buf, '(');
    pos_end = strchr(buf, ')');
    // ==========================================
    *pos_end = '\0';
    //	printf("%s\n", pos_start);
    // =========================================
    for (i = 0; i < 6; i++)
    {
        part[i] = (char *)malloc(sizeof(char) * 4);
    }
    for (i = 0; i < 6; i++)
    {
        pos_start++;
        pos_cur = strchr(pos_start, ',');
        if (pos_cur)
            *pos_cur = '\0';
        //		printf("get part: %s\n", pos_start);
        strcpy(part[i], pos_start);
        pos_start = pos_cur;
    }
    free(buf);
    sprintf(dest_ip, "%s.%s.%s.%s", part[0], part[1], part[2], part[3]);
    data_port = atoi(part[4]) * 256 + atoi(part[5]);
    printf("connect to %s:%d\n", dest_ip, data_port);
    if ((host_ent = gethostbyname(dest_ip)) == NULL)
    {
        perror("gethostbyname");
        printf("...error gethostbyname\n");
        return -1;
    }
    for (i = 0; i < 6; i++)
    {
        free(part[i]);
    }
    printf("creating data sock....\n");
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        printf("...Error socket");
        return -1;
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data_port);
    server_addr.sin_addr = *((struct in_addr *)host_ent->h_addr);
    printf("connecting.....\n");
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        printf("...error connect");
        return (-1);
    }
    printf("create data sock successful\n");
    return sockfd;
}

//send command to server with sock_fd
int ftp_send_cmd(const char *s1, const char *s2, int sock_fd)
{
    char send_buf[256];
    int err = -1;
    int len = 0;
    if (s1)
    {
        strcpy(send_buf, s1);
        if (s2)
        {
            strcat(send_buf, s2);
            strcat(send_buf, "\r\n");
            len = strlen(send_buf);
            err = send(sock_fd, send_buf, len, 0);
        }
        else
        {
            strcat(send_buf, "\r\n");
            len = strlen(send_buf);
            err = send(sock_fd, send_buf, len, 0);
        }
    }
    if (err < 0)
        printf("send() error!\n");

    return err;
}

//get the server's reply message from sock_fd
int ftp_get_reply(int sock_fd)
{
    static int reply_code = 0, count = 0;
    char rcv_buf[512];
    count = read(sock_fd, rcv_buf, 510);
    if (count > 0)
        reply_code = atoi(rcv_buf);
    else
        return 0;
   // uint32_t time_read_count = xTaskGetTickCount();
    while (1)
    {
        if (count <= 0)
            break;
        rcv_buf[count] = '\0';
        printf("%s", rcv_buf);
        count = read(sock_fd, rcv_buf, 510);
        // if ((xTaskGetTickCount() - time_read_count) > 100 * 20) //20s
        //     break;
    }
    return reply_code;
}

int create_port_data_sock()
{
    return xconnect_ftpdata();
}

int get_port()
{
    char port_respond[512];
    char *buf_ptr;
    int count, port_num;
    ftp_send_cmd("PASV", NULL, sock_control);
    count = read(sock_control, port_respond, 510);
    if (count <= 0)
        return 0;
    port_respond[count] = '\0';
    if (atoi(port_respond) == 227)
    {
        //get low byte of the port
        buf_ptr = strrchr(port_respond, ',');
        port_num = atoi(buf_ptr + 1);
        *buf_ptr = '\0';
        //get high byte of the port
        buf_ptr = strrchr(port_respond, ',');
        port_num += atoi(buf_ptr + 1) * 256;
        return port_num;
    }
    return 0;
}

int rand_local_port()
{
    int local_port;
    srand((unsigned)time(NULL));
    local_port = rand() % 40000 + 1025;
    return local_port;
}

//connect data stream
int xconnect_ftpdata()
{
    if (mode)
    {
        int data_port = get_port();
        if (data_port != 0)
            ftp_server.sin_port = htons(data_port);
        return (xconnect(&ftp_server, 0));
    }
    else
    {
        int client_port, get_sock, opt, set;
        char cmd_buf[32];
        struct timeval outtime;
        struct sockaddr_in local;
        char local_ip[24];
        char *ip_1, *ip_2, *ip_3, *ip_4;
        int addr_len = sizeof(struct sockaddr);
        client_port = rand_local_port();
        get_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (get_sock < 0)
        {
            // cmd_err_exit("socket()", 1);
            printf("...errr get_sock \n");
            return -1;
        }

        //set outtime for the data socket
        outtime.tv_sec = 7;
        outtime.tv_usec = 0;
        opt = SO_REUSEADDR;
        set = setsockopt(get_sock, SOL_SOCKET, SO_RCVTIMEO,
                         &outtime, sizeof(outtime));
        if (set != 0)
        {
            printf("set socket %s errno:%d\n", strerror(errno), errno);
            printf("...errr set socket \n");
            return -1;
        }
        set = setsockopt(get_sock, SOL_SOCKET, SO_REUSEADDR,
                         &opt, sizeof(opt));
        if (set != 0)
        {
            printf("set socket %s errno:%d\n", strerror(errno), errno);
            printf("...errr set socket \n");
            return -1;
        }

        bzero(&local_host, sizeof(local_host));
        local_host.sin_family = AF_INET;
        local_host.sin_port = htons(client_port);
        local_host.sin_addr.s_addr = htonl(INADDR_ANY);
        bzero(&local, sizeof(struct sockaddr));
        // uint32_t time_connect_count = xTaskGetTickCount();
        while (1)
        {
            set = bind(get_sock, (struct sockaddr *)&local_host,
                       sizeof(local_host));
            if (set != 0 && errno == 11)
            {
                client_port = rand_local_port();
                // if ((xTaskGetTickCount() - time_connect_count) > 100 * 20) //20s
                // {
                //     printf("...error bind timeout > 20s\n");
                //     return -1;
                // }

                continue;
            }
            set = listen(get_sock, 1);
            if (set != 0 && errno == 11)
            {
                // cmd_err_exit("listen()", 1);
                printf("... error listen\n");
                return -1;
            }
            //get local host's ip
            if (getsockname(sock_control, (struct sockaddr *)&local,
                            (socklen_t *)&addr_len) < 0)
                return -1;
            snprintf(local_ip, sizeof(local_ip), inet_ntoa(local.sin_addr));
            //change the format to the PORT command needs.
            local_ip[strlen(local_ip)] = '\0';
            ip_1 = local_ip;
            ip_2 = strchr(local_ip, '.');
            *ip_2 = '\0';
            ip_2++;
            ip_3 = strchr(ip_2, '.');
            *ip_3 = '\0';
            ip_3++;
            ip_4 = strchr(ip_3, '.');
            *ip_4 = '\0';
            ip_4++;
            snprintf(cmd_buf, sizeof(cmd_buf), "PORT %s,%s,%s,%s,%d,%d",
                     ip_1, ip_2, ip_3, ip_4, client_port >> 8, client_port & 0xff);
            ftp_send_cmd(cmd_buf, NULL, sock_control);
            if (ftp_get_reply(sock_control) != 200)
            {
                printf("Can not use PORT mode!Please use \"mode\" change to PASV mode.\n");
                return -1;
            }
            else
                return get_sock;
        }
    }
}

char kTfpIp[30] = "47.105.217.224"; //for ftb ota download


 int do_firmware_upgrade_ftp(char *url, uint32_t total_length, uint32_t crc32)
{

    int download_len = ftp_download(1, url);

    if (download_len != total_length)
    {
        usleep(600 * 1000);
        download_len = ftp_download(1, url);
    }
    if (download_len != total_length)
    {
        usleep(600 * 1000);
        download_len = ftp_download(1, url);
    }

    if (download_len != total_length)
    {
        printf("...Error Download len:%d != need :%d", download_len, total_length);
        return -1;
    }
    else
    {
        printf("...OK Download len:%d, == need :%d", download_len, total_length);

        uint8_t *pBuf = (uint8_t *)buffer;

        uint32_t download_crc32 = 0;
        download_crc32 = cal_crc32(download_crc32, pBuf, download_len);

        if (download_crc32 != crc32)
        {
            printf("\n\t Error_Crc ap_download[%d].crc32 = %u, but need is %u", 1, download_crc32, crc32);
            return -1;
        }
        else
        {
            printf("crcok");
            return 0;
        }
    }
}

int ftp_download(int uart_num, char* filename)
{
    if(kIsFtpInit != 1)
        ftp_init(kTfpIp,21);

    if(kIsFtpInit != 1)
        ftp_init(kTfpIp, 21);
    if(kIsFtpInit != 1)
        ftp_init(kTfpIp, 21);

    if (kIsFtpInit != 1)
    {
        printf("\n...init fail!");
        return -1;
    }

    // notify_reset(uart_num);
    
    // ====================================================
    // some pre-process work may be done here
    // ====================================================

    int sockfd = -1;
    int err = -1;
    char cmd[100];

    if (-1 == (sockfd = create_data_sock()))
    {
        printf("...Error create_data_sock\n");
        return -1;
    }
    printf("\n...create_data_sock");

    err = set_bin_mode(1, sock_control);
    if (err == -1)
    {
        printf("...Set bin mode fail\n");
        if (sockfd != -1)
            close(sockfd);
        return -1;
    }
    else
    {
        printf("\n...set bin mode");
    }
    

    sprintf(cmd, "RETR %s\r\n", "053274c0-2b1f-48f4-bcb5-752e04892775.bin");
    int ret_value = send_command(cmd, NULL, sock_control);
    if (!(ret_value == 150 || ret_value == 125))
    {
        // fprintf(stderr, buf);
        printf("!(ret_value == 150 || ret_value == 125 \n");
        close(sockfd);
        return (-1);
    }
    else
    {
        printf("\n...send_command success");
    }
    
    if (!mode && (-1 == (sockfd = get_port_data_connection(sockfd))))
    {
        printf("...get_port_data_connectio fail\n");
        close(sockfd);
        return (-1);
    }
    else
    {
        printf("\n...get_port_data_connectio success");
    }
    
    int get_size;
    get_size = file_copy(sockfd, uart_num);
    close(sockfd);
    send_command(NULL, NULL, sock_control);
   // ftp_quit();
    printf("received with %d bytes\n", get_size);

    return get_size;
}

int file_copy(int srcfd, int uart_num)
{
    char *buf;

    int read_size = 0;
    int total_size = 0;
    if (-1 == srcfd)
    {
        fprintf(stderr, "fd error");
        printf("...fd error\n");
        return -1;
    }
    buf = (char *)malloc(sizeof(char) * BUFSIZE);
    memset(buf, 0, sizeof(char) * BUFSIZE);
    int crc =0;
    while (0 != (read_size = read(srcfd, buf, sizeof(char) * BUFSIZE)))
    {
        printf("file_copy: read_size: %d\n", read_size);
        if (read_size == -1 && errno != EINTR)
        {
            break;
        }
        else if (read_size > 0)
        {

            crc = cal_crc32(crc,buf,read_size);
            memcpy(&buffer[total_size], buf, read_size);
            total_size += read_size;
             
            // if (uart_num == 0)
            //     fifo_in(g_mqtt_uart0_fifo, (char *)(buf), read_size);
            // else if (uart_num == 1)
            //     fifo_in(g_mqtt_uart1_fifo, (char *)(buf), read_size);
            // else if (uart_num == 2)
            //     fifo_in(g_mqtt_uart2_fifo, (char *)(buf), read_size);
        }
        memset(buf, 0, sizeof(char) * BUFSIZE);
    }

    printf("total_len:[%d,%x], crc[%d,%x]\n", total_size, total_size, crc,crc);
    free(buf);
    return total_size;
}

int get_port_data_connection(int sockfd)
{
    int i = 0;
    int new_sock = -1;
    int set = 0;
    struct sockaddr_in local_host;
    memset(&local_host, 0, sizeof(struct sockaddr_in));
    for (i = 0; i < 3; i++)
    {
        new_sock = accept(sockfd, (struct sockaddr *)&local_host, (socklen_t *)&set);
        if (new_sock == -1)
        {
            printf("accept return:%s errno: %d\n", strerror(errno), errno);
            continue;
        }
        else
            break;
    }
    close(sockfd);
    if (new_sock == -1)
    {
        printf("Sorry, you can't use PORT mode. There is something wrong when the server connect to you.\n");
    }
    return new_sock;
}

int set_bin_mode(int mode, int sockfd)
{
    int ret_value = 0;
    char buf[BUFSIZE];
    if (mode)
    {
        ret_value = send_command("TYPE I\r\n", buf, sockfd);
    }
    else
    {
        ret_value = send_command("TYPE A\r\n", buf, sockfd);
    }
    if (ret_value != 200)
    {
        printf(buf);
        printf("\n");
        return (-1);
    }
    else
    {
        return (0);
    }
}
//login to the server
int ftp_login()
{
    int err;
    if (ftp_send_cmd("USER ", user, sock_control) < 0)
    {
        printf("...Can not sendã€€user message\n");
        return -1;
    }
    err = ftp_get_reply(sock_control);
    if (err == 331)
    {
        if (ftp_send_cmd("PASS ", passwd, sock_control) <= 0)
        {
            printf("...Can not sendã€€user message\n");
            return -1;
        }
        else
            err = ftp_get_reply(sock_control);
        if (err != 230)
        {
            printf("Password error!\n");
            return -1;
        }
        return 1;
    }
    else
    {
        printf("User error!\n");
        return -1;
    }
}

//call this function to quit
void ftp_quit()
{
    ftp_send_cmd("QUIT", NULL, sock_control);
    ftp_get_reply(sock_control);
    close(sock_control);
}

int ftp_init(char *hostip, int port)
{
    printf("\n...ftp init");
    printf("\n...ftp_download hostip:%s, port:%d\n", hostip, port);
    char cmd[100];
    int err;

    err = fill_host_addr(hostip, &ftp_server, port);
    if (err == 254)
    {
        printf("\n...Invalid port!");
        return -1;
    }

    if (err == 253)
    {
        printf("\n...Inalid server address!");
        return -1;
    }

    sock_control = xconnect(&ftp_server, 1);
    if (sock_control == -1)
        return -1;
    if ((err = ftp_get_reply(sock_control)) != 220)
    {
        printf("\n...Connect error!");
        if (sock_control != -1)
            close(sock_control);
        return -1;
    }

    err = ftp_login();
    if (err != 1)
    {
        printf("\n...ftp login errr\n");
        if (sock_control != -1)
            close(sock_control);
        return -1;
    }

    kIsFtpInit = 1;
    printf("\n...init finish");
    
    return err;

}