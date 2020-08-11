#include "tcpclient.h"

TCPClient::TCPClient(string _ip, int _port, ClientReadCallback _readcallback, ClientConnectCallback _connectcallback, ClientDisconnectCallback _disconnectcallback):
    ip(_ip),
    port(_port),
    readcallback(_readcallback),
    connectcallback(_connectcallback),
    disconnectcallback(_disconnectcallback)
{
    
}

TCPClient::connect()
{

}

