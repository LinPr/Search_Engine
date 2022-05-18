#include "Thread.h"
#include <stdio.h>

Thread::Thread(ThreadCallback &&cb)
: _thid(0)
, _isRunning(false)
, _cb(std::move(cb))//注册回调函数
{

}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_thid);
        _isRunning = false;
    }
}

//pthread->start();
void Thread::start()
{
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret)
    {
        perror("pthread_create");
        return;
    }

    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_thid, nullptr);
        _isRunning = false;
    }
}

void *Thread::threadFunc(void *arg)
{
    Thread *pth = (Thread *)arg;
    if(pth)
    {
        pth->_cb();//执行回调函数
    }

    pthread_exit(nullptr);
}
