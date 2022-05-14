#include "TcpConnection.h"
#include "EventLoop.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <functional>

TcpConnection::TcpConnection(int fd, EventLoop * peventLoop)
: _connfd(fd)
, _sockIO(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
, _peventLoop(peventLoop)
{
    std::cout << "TcpConnection(int fd)" << std::endl;
}

/* 按照类成员的声明顺序析构，类似压栈和出栈 */
TcpConnection::~TcpConnection()
{
    std::cout << "~TcpConnection()" << std::endl;
}

InetAddress TcpConnection::getLocalAddr() const
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);

    int ret = getsockname(_connfd.fd(), (struct sockaddr*)&addr, &len);
    if(-1 == ret) { perror("getsockname error"); exit(-1); }
    
    return addr;
}

InetAddress TcpConnection::getPeerAddr() const
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);

    int ret = getpeername(_connfd.fd(), (struct sockaddr*)&addr, &len);
    if(-1 == ret) { perror("getsockname error"); exit(-1); }
    
    return addr;
}

void TcpConnection::sendData(const std::string & msg)
{
    _sockIO.writen(msg.c_str(), msg.size());
}

std::string TcpConnection::receiveData()
{
    char BUF[65535] = {0};
    _sockIO.readLine(BUF, sizeof(BUF));
    return std::string(BUF);
}

// 将两端地址信息转换成字符串输出
std::string TcpConnection::connectionInfo()
{
    std::ostringstream oss;
    oss << _localAddr.ip() << ":" << _localAddr.port() 
        << "--->"
        << _peerAddr.ip() << ":" << _peerAddr.port();

    return oss.str();
}

bool TcpConnection::isPeerClosed() const
{
    char BUF[1024] = {0};
    int ret = ::recv(_connfd.fd(), BUF, sizeof(BUF), MSG_PEEK); //注意 flag = peek
    if(ret)  { return false; }
    return true;
}

void TcpConnection::setEstablishCallback(const TcpConnectionCallback & cb)
{
    _onEstablish_cb = cb;
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback & cb)
{
    _onMessage_cb = cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback & cb)
{
    _onClose_cb = cb;
}

void TcpConnection::handleEstablishCallback()
{
    /* TcpConnectionCallback = function< void(const TcpConnectionPtr &) >; */
    //shared 返回对象的一个智能指针，对比裸指针this，相当于this封装成智能指针
    /* shared_from_this() 返回一个 shared_ptr<TcpConnection> 对象 ;*/
    /*  _onMessage_cb 类型为 function< void(const TcpConnectionPtr &) > */

    _onEstablish_cb(shared_from_this());
    // std::cout << "_onMessage_cb(shared_from_this());" << std::endl;
}

void TcpConnection::handleMessageCallback()
{
    _onMessage_cb(shared_from_this());
}

void TcpConnection::handleCloseCallback()
{
    _onClose_cb(shared_from_this());
}

/*将 IO 任务从整个任务中抽离出来，注册到Reactor中的IO任务队列中，等时机合适时再执行*/
void TcpConnection::registerIOtask(const std::string & msg)
{
    if(_peventLoop)
    {
// 这里不同的 TCPConnection 对象不同，虽然bind返回形式相同，底层却指向不同对象的成员函数 
        IOTask IOtaskCallback = std::bind(&TcpConnection::sendData, this, msg);
        _peventLoop->addIOtask2List(IOtaskCallback);
    }
}