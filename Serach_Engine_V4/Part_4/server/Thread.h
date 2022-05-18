#pragma once 

#include "NonCopyable.h"
#include <pthread.h>
#include <functional>

class Thread
:NonCopyable
{
    using ThreadCallback = std::function<void()>;
public:
    Thread(ThreadCallback cb);
    ~Thread();

    void start();
    void join();

    pthread_t get_pthreadID();

private: 
    
    static void * threadFunc(void * arg);
    ThreadCallback _cb;

private:
    pthread_t _thid;    
    bool _isRunnig;
};