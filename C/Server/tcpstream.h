/*********************************************************************************
 *(C) @@.        [] All Rights Reserved.
 *FileName:      tcpstream.h
 *Author:        flyfly
 *Telephtone:    
 *Version:       1.0.0
 *Data:          2020.08.11
 *Description:   
 *Others:        
 *History:       
**********************************************************************************/
#ifndef __TCPSTREAM_H__
#define __TCPSTREAM_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

using namespace std;

class TCPStream
{
    public:

        enum {
            CONNECT_CLOSED = 0,
            CONNECT_RESET = -1,
            CONNECT_TIMEOUT = -2,
        };

        TCPStream(int sd, struct sockaddr_in* addr);
        ~TCPStream();
    
        ssize_t send(const char* buffer, size_t len);
        ssize_t receive(char* buffer, size_t len, int timeout=0);
    private:
        int sd;
        string ip;
        int port;


        bool waitForReadEvent(int timeout);

};


#endif // !__TCPSTREAM_H__

