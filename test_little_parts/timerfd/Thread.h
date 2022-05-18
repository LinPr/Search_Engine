#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include <functional>

using std::function;

class Thread
{
    using ThreadCallback = function<void()>;
public:
    Thread(ThreadCallback &&cb);
    ~Thread();
    void start();
    void join();

private:
    //消除this指针的影响
    static void *threadFunc(void *);
private:
    pthread_t _thid;
    bool _isRunning;
    ThreadCallback _cb;
};

#endif
