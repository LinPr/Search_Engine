#pragma once

#include "InetAddress.h"
#include "Socket.h"
#include <string>

/* 用于管理 server 端套接字监听和连接 */
class Acceptor
{
public:
    Acceptor(const std::string ip, unsigned short port);
    ~Acceptor();

    void bind();
    void listen();
    int accept();

    void setReuseAddr();
    void setReusePort();

    void ready();
    int listenfd();

private:
    Socket _listenSock; // 组合关系
    InetAddress _servAddr;
    int _connfd;
};

