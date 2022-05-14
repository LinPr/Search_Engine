#pragma once

#include "Acceptor.h"
#include "EventLoop.h"

class TcpServer
{
public:
    TcpServer(const std::string & ip, unsigned short port);
    ~TcpServer();

    void tcpServerStart();
    void tcpServerStop();

    void registerAllCallback(const TcpConnectionCallback & Establish, const TcpConnectionCallback & Message, const TcpConnectionCallback &  Close);

private:
    Acceptor  _acceptor;
    EventLoop _eventLoop;
};


