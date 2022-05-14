#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)


ThreadPool::ThreadPool(int threadsNum, int queSize)
: _threadsNum(threadsNum)
, _threads()
, _taskQueue(queSize)
, _isExit(false)
{
    _threads.reserve(_threadsNum);
    std::cout << "ThreadPool(int threadsNum, int queSize)" << std::endl;
}

ThreadPool::~ThreadPool()
{
    if(!_isExit)
    {
        stopPool();
    }
    std::cout << "~ThreadPool()" << std::endl;
}

void ThreadPool::addTask(const ElementType task)
{
    if(task)
    {
        _taskQueue.push(task);
    }
}

ElementType ThreadPool::getTask()
{
    return _taskQueue.pop();
}

void ThreadPool::startPool()
{
    for(int i = 0; i < _threadsNum; ++i)
    {
        std::unique_ptr<Thread> pthread(new Thread(std::bind(&ThreadPool::taskExcute, this)));
        _threads.push_back(std::move(pthread));

        _threads[i]->start();
    }
}

void ThreadPool::stopPool()
{
    while(!_taskQueue.empty()) {}; // 等待任务全部执行完成

    _isExit = true;

    _taskQueue.threadsWakeup();
    for(auto & th : _threads)
    {
        th->join();
    }
}


void ThreadPool::taskExcute()
{
    while(!_isExit) // 如果没有调用stop，线程就一直工作
    {   
        /* 解决阻塞的方法：
                1. 阻塞之前：从源头防止可能阻塞的因素
                2. 阻塞之后：阻塞之后如何唤醒 */
        // if(!_taskQueue.empty())
        ElementType task = getTask(); // 这里可能会阻塞
        if(task) 
        { 
            task();
            std::cout << "ThreadPool::taskExcute() = " << gettid() << std::endl;
        }
    }    
}