#include "Thread.h"
#include <stdio.h>
#include <iostream>


Thread::Thread(ThreadCallback cb)
: _thid(0)
, _isRunnig(false)
, _cb(cb) // 注册回调
{
    std::cout << "Thread::Thread(ThreadCallback && cb)" << std::endl;
}

Thread::~Thread()
{
    if(_isRunnig)
    {
        pthread_detach(_thid);
        _isRunnig = false;
    }
    std::cout << "~Thread()" << std::endl;
}

void Thread::start()
{
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret) { perror("pthread_create"); return; } 

    _isRunnig = true;
}

void Thread::join()
{
    if(_isRunnig)
    {
        int ret = pthread_join(_thid, nullptr);
        if(ret) { perror("pthread_join"); return; }

        _isRunnig = false;
    }
}

pthread_t Thread::get_pthreadID()
{
    return _thid;
}

void * Thread::threadFunc(void * arg)
{
    Thread * pth = (Thread *)arg;
    if(pth)
    {
        pth->_cb();
    }
    // pthread_exit(nullptr);
}
 