#include "tcpclient.h"
#include <iostream>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

TCPClient::TCPClient(string _ip, int _port, ClientReadCallback _readcallback, ClientConnectCallback _connectcallback, ClientDisconnectCallback _disconnectcallback):
    ip(_ip),
    port(_port),
    readcallback(_readcallback),
    connectcallback(_connectcallback),
    disconnectcallback(_disconnectcallback),
    reconnect(true),
    quit(false)
{
    
}

TCPClient::~TCPClient()
{

    quit = true;
    if(t && t->joinable())
        t->join();
    //? delete t;
}

void TCPClient::connect()
{
    t = new thread(&TCPClient::threadProcess, this);
}

void TCPClient::threadProcess()
{
    char recvBuf[TCPCLIENT_RECV_BUF_LEN];
    struct sockaddr_in addr;
    int sd;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if(resolveHostName(ip.c_str(), &(addr.sin_addr)) != 0)
    {

    }


    while(!quit)
    {
        if(!reconnect)
        {
            std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(50));
            continue;
        }

        try
        {
            sd = socket(AF_INET, SOCK_STREAM, 0);
            if(sd <0 )
            {
                perror("socket() failed");
                continue;
            }

            if(::connect(sd, (struct sockaddr*)&addr, sizeof(addr))!=0)
            {
                perror("connect() failed");
                close(sd);
                std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000));
                continue;
            }
            else
            {
                stream = new TCPStream(sd, &addr);
                if(stream && connectcallback)
                    connectcallback();

                size_t recv_len;
                while(!quit && stream) //一直读吗， 没有停顿时间？？？
                {
                    recv_len = stream->receive(recvBuf, sizeof(recvBuf));
                    if(recv_len > 0 && readcallback)
                        readcallback(recvBuf, recv_len);
                }

            }

            if(disconnectcallback)
                disconnectcallback();            
            
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';

            if(disconnectcallback)
                disconnectcallback();
        }
        
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000));

    }

}


int TCPClient::resolveHostName(const char* hostname, struct in_addr* addr)
{
    struct addrinfo *res;
    int result = getaddrinfo(hostname, NULL, NULL, &res);
    if(result == 0)
    {
        memcpy(addr, &((struct sockaddr_in *) res->ai_addr)->sin_addr, sizeof(struct in_addr));
        freeaddrinfo(res);
    }

    return result;
} 
void TCPClient::join()
{
    t->join();
}