#include "Socket.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Socket::Socket()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if( -1 == _fd)  { perror("socket error"); exit(-1); }
}

Socket::Socket(int & fd)
: _fd(fd)
{

}

Socket::~Socket()
{
    close(_fd);
}

int Socket::fd() const
{
    return _fd;
}

void Socket::shutDwonWrite()
{
    int ret = shutdown(_fd, SHUT_WR);
    if(-1 == ret) { perror("hutdown error"); exit(-1); }
}