#pragma once 

#include "Thread.h"
#include "TaskQueue.h"
#include "LRUCachesManager.h"
#include <memory>
#include <iostream>


using ElementType = std::function<void()>;

class ExitTest
{
public:
    ExitTest()
    {
        std::cout << "ExitTest()" << std::endl;
    }
    ~ExitTest()
    {
        std::cout << "~ExitTest()" << std::endl;
    }
};

class ThreadPool
{
public:
    ThreadPool(int threadsNum, int queSize, int CacheCapacity);
    ~ThreadPool();

    void addTask(const ElementType task);
    ElementType getTask();
    void startPool();
    void stopPool();


    LRUCachesManager & get_cachesManager() { return _cachesManager; };

private:
    void taskExcute();

private:
   
    size_t _threadsNum;
    std::vector<std::unique_ptr<Thread>> _threads; // 组合关系，负责部件的生命周期
    ExitTest e;
    TaskQueue _taskQueue;
    bool _isExit;

    LRUCachesManager _cachesManager;
};

