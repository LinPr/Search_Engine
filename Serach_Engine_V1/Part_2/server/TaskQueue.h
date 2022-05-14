#pragma once 

#include "MutexLock.h"
#include "Condition.h"
#include "Task.h"
#include <queue>
#include <functional>
#include <iostream>

using ElementType = std::function<void()>;

class ExitTest2
{
public:
    ExitTest2()
    {
        std::cout << "ExitTest2()" << std::endl;
    }
    ~ExitTest2()
    {
        std::cout << "~ExitTest2()" << std::endl;
    }
};

class TaskQueue
{

public:
    TaskQueue(size_t queSize);
    ~TaskQueue();

    void push(ElementType value);
    ElementType pop();
    bool full();
    bool empty();
    void threadsWakeup();
private:
    
    ExitTest2 e; // 这个没有被析构掉为什么？
    std::queue<ElementType> _que;    // 这个没有被析构掉为什么？
    size_t _queSize;
    MutexLock _mutex;
    Condition _notFull;  //注意声明顺序 condition 用到 mutex 
    Condition _notEmpty;
// 两个condition， 一个用于生产者通知消费者， 另一个用于消费者通知生产者
    bool _notifyAllFlag;
};


