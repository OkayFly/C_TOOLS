/*********************************************************************************
 *(C) @@.        [] All Rights Reserved.
 *FileName:      tcpserver.cpp
 *Author:        flyfly
 *Telephtone:    
 *Version:       1.0.0
 *Data:          2020.08.13
 *Description:   
 *Others:        
 *History:       
**********************************************************************************/
#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <string>
#include <netinet/in.h>
#include "tcpstream.h"

using namespace std;

class TCPServer
{

    public:
        TCPServer(int port, const char* address= "");
        ~TCPServer();

    

};

#endif //__TCPSERVER_H__
