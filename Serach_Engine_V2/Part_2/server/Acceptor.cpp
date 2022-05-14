#include "Acceptor.h"

Acceptor::Acceptor(const std::string ip, unsigned short port)
: _listenSock()
, _servAddr(ip, port)
{

}

Acceptor::~Acceptor()
{
}


void Acceptor::bind()
{
    int ret = ::bind(_listenSock.fd(), (struct sockaddr*)_servAddr.getInetAddress(), sizeof(struct sockaddr));
    if(-1 == ret) { perror("bind error"); exit(-1); }
}

void Acceptor::listen()
{
    int ret = ::listen(_listenSock.fd(), 128);
    if(-1 == ret) { perror("listen error"); exit(-1); }
}

int Acceptor::accept()
{
    int connfd = ::accept(_listenSock.fd(), nullptr, nullptr);
    if(connfd < 0) { perror("accept error"); exit(-1); }
    return connfd;
}

void Acceptor::setReuseAddr()
{
    int opt = 1;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(-1 == ret)
    {
        perror("setsockopt ip error");
        exit(-1);
    }
}
void Acceptor::setReusePort()
{
    int opt = 1;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    if(-1 == ret)
    {
        perror("setsockopt port error");
        exit(-1);
    }
}


void Acceptor::ready()
{
    setReuseAddr(); // 先设置网络文件描述符属性属性
    setReusePort();

    bind();
    listen();
}

int Acceptor::listenfd()
{
    return _listenSock.fd();
}