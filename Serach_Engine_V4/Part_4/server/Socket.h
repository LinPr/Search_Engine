#pragma once 

#include "NonCopyable.h"

/* 管理 sockfd */
class Socket
: NonCopyable
{
public:
    Socket(); // 创建套接字
    Socket(int & fd); // 拷贝套接字(这里不要加const，只拷贝左值)，把有参构造当拷贝构造来用
    ~Socket();

    int fd() const;
    void shutDwonWrite();

private:
    int _fd;
};


