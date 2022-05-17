#pragma once 

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include <memory>
#include <functional>

// forward declaration 
class EventLoop;

class TcpConnection
:public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using TcpConnectionCallback = std::function< void(const TcpConnectionPtr &) >;

public:
    TcpConnection(int fd, EventLoop * eventLoop);
    ~TcpConnection();
    void sendData(const std::string & meg);
    std::string receiveData();
    std::string connectionInfo();

    bool isPeerClosed() const;
    void setEstablishCallback(const TcpConnectionCallback & cb);
    void setMessageCallback(const TcpConnectionCallback & cb);
    void setCloseCallback(const TcpConnectionCallback & cb);
    
    void handleEstablishCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    TcpConnectionCallback _onEstablish_cb;
    TcpConnectionCallback _onMessage_cb;
    TcpConnectionCallback _onClose_cb;
private:
    InetAddress getLocalAddr() const;
    InetAddress getPeerAddr() const;

private:
    /* 都是组合关系 */

    Socket _connfd; 
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;

/* 引入线程池 和 Reactor */
public:
    void registerIOtask(const std::string & msg);

private:
    EventLoop * _peventLoop; // 传指针类型是因为可以传 this，
};


