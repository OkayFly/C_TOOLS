// 定时器，每2分钟发送 helloword 到服务器

#include <iostream>
#include <stdio.h>
#include "tcpclient.h"



void onRead(const char *data, int len)
{
    std::cout << "Receive:"<<data << "len:"<<len<<std::endl;
}

void onConnect()
{
    std::cout << "client connect "<<std::endl;
}

void onDisconnect()
{
    std::cout<<"client disconnect" << std::endl;
}


int main(int argc, char** argv)
{
    if(argc !=3) 
    {
        printf("usage: %s <port> <ip>", argv[0]);
        exit(1);
    }



    TCPClient* tcp_client = new TCPClient("127.0.0.1", 21, onRead, onConnect, onDisconnect);


    while(1)
    {
        

    }

    return 0;
}