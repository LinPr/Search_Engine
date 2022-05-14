#include "TcpServer.h"
#include <iostream>

TcpServer::TcpServer(const std::string & ip, unsigned short port)
: _acceptor(ip, port)
, _eventLoop(_acceptor)
{
    std::cout << "TcpServer(const std::string & ip, unsigned short port)" << std::endl;
}

TcpServer::~TcpServer()
{
    std::cout << "~TcpServer()" << std::endl;
}


void TcpServer::tcpServerStart()
{
    _acceptor.ready();
    _eventLoop.startLoop();
}

void TcpServer::tcpServerStop()
{
    _eventLoop.stopLoop();
}


void TcpServer::registerAllCallback(const TcpConnectionCallback & Establish, 
                                    const TcpConnectionCallback & Message,
                                    const TcpConnectionCallback & Close)
{
    _eventLoop.setEstablishCallback(Establish);
    _eventLoop.setMessageCallback(Message);
    _eventLoop.setCloseCallback(Close);
}