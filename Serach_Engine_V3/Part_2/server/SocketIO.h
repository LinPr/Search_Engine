#pragma once 


/* 套接字数据的发送和接收 */
class SocketIO
{
public:
    SocketIO(int fd);
    int readn(char *buf, int len);
    int writen(const char *buf, int len);
    int readLine(char *buf, int maxLen);

private:
    int _fd;
};


