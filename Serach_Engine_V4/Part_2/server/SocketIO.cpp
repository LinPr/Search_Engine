#include "SocketIO.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>


SocketIO::SocketIO(int fd)
: _fd(fd)
{

}


int SocketIO::readn(char *buf, int len)
{
    int left = len;
    char *ptr = buf;
    int ret = 0;

    while(left > 0)
    {
        ret = recv(_fd, ptr, left, 0);
        /* ret = read(_fd, ptr, left); */
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("SocketIO readn == -1 error");
            return len - ret;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            ptr += ret;
            left -= ret;
        }
    }

    return len - left;
}

int SocketIO::writen(const char *buf, int len)
{   // 因为内核缓冲区容量有限，可能需要分多次发送

    int left = len;
    const char *ptr = buf;
    int ret = 0;

    while(left > 0)
    {
        ret = write(_fd, ptr, left);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("SocketIO writen == -1 error");
            return len - ret;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            ptr += ret;
            left -= ret;
        }
    }

    return len - left;
}

int SocketIO::readLine(char *buf, int maxLen)
{
    int left = maxLen - 1; // 缓冲区剩于可用容量
    char *ptr = buf;
    int ret = 0, total = 0;

    // 不超过缓冲区最大容量
    while(left > 0)
    {
        // 拷贝内核缓冲数据到用户缓冲，但并不立即删除内核缓冲的原始数据，要做\n判断
        ret = recv(_fd, ptr, left, MSG_PEEK); 
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("SocketIO readn == -1 error");
            return maxLen - ret;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            for(int idx = 0; idx < ret; ++idx)
            {
                if(ptr[idx] == '\n')
                {
                    int sz = idx + 1;
                    readn(ptr, sz); // 读取，并删除内核缓冲数据
                    ptr += sz; //注意用户缓冲区指针移动的时机
                    *ptr = '\0';

                    return total + sz;
                }
            }

            readn(ptr, ret);//从内核态拷贝到用户态
            total += ret;
            ptr += ret;
            left -= ret;
        }
    }

    *ptr = '\0';

    return total - left;

}
