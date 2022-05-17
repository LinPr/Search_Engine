#include "TaskQueue.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#include <iostream>

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _notifyAllFlag(false)
{
    std::cout << "TaskQueue::TaskQueue(size_t queSize)" << std::endl;
}

TaskQueue::~TaskQueue()
{
    std::cout << "~TaskQueue()" << std::endl;
}


void TaskQueue::push(ElementType task)
{
    MutexLockGuard autoLock(_mutex);

    while(full())
    {
        _notFull.wait();
    }
    _que.push(task);

    _notEmpty.notify();
}

ElementType TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);

    while((!_notifyAllFlag) && empty() )
    {
        _notEmpty.wait();
    }

    if(_notifyAllFlag == true)
    {
        std::cout << "thread wakeup = " << gettid() << std::endl;
    }
    if(!_que.empty())
    {
        ElementType tmp = _que.front();
        _que.pop();
        _notFull.notify();

        return tmp; // 对空的队列进行去front返回的是nullptr；
    }
    return nullptr;
    
}

bool TaskQueue::full()
{
    return _que.size() == _queSize;
}

bool TaskQueue::empty()
{
    return _que.size() == 0;
}

void TaskQueue::threadsWakeup()
{
    _notifyAllFlag = true; // 先设置成 false

    std::cout << "_notEmpty.notifyAll()" << std::endl;
    _notEmpty.notifyAll(); 
}