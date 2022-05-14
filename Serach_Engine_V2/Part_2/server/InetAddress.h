#pragma once 

#include <arpa/inet.h>
#include <string>

/* 用于管理 IP地址 + 端口号 */
class InetAddress
{
public:
    InetAddress(const std::string ip, unsigned short port);
    InetAddress(const struct sockaddr_in addr);
    ~InetAddress();

    std::string ip() const;
    unsigned short port() const;

    const struct sockaddr_in* getInetAddress() const;

private:
    struct sockaddr_in _addr;
};

