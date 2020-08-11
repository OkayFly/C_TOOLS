/*********************************************************************************
 *(C) @@.        [] All Rights Reserved.
 *FileName:      tcpclient.h
 *Author:        flyfly
 *Telephtone:    
 *Version:       1.0.0
 *Data:          2020.08.11
 *Description:   for linux reference to https://github.com/vichargrave/tcpsockets
 *Others:        
 *History:       
**********************************************************************************/

#ifndef __TCPCLIENT__H__
#define __TCPCLIENT__H__

#include <netinet/in.h>
#include <functional>
#include "tcpstream.h"


using namespace std;

class TCPClient
{

    public:

        typedef std::function<void (const char*data, int len)> ClientReadCallback;
        typedef std::function<void ()> ClientConnectCallback;
        typedef std::function<void()> ClientDisconnectCallback;

        TCPClient(string _ip, int _port, ClientReadCallback _readcallback, ClientConnectCallback _connectcallback, ClientDisconnectCallback _disconnectcallback);
        ~TCPClient();

        TCPStream* connect();

    private:
        string ip;
        int port;

        ClientReadCallback readcallback;
        ClientConnectCallback connectcallback;
        ClientDisconnectCallback disconnectcallback;



};

#endif // !__TCPCLIENT__H__


