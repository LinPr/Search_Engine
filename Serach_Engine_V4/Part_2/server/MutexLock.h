#pragma once

#include "NonCopyable.h"
#include <pthread.h>

class MutexLock
:NonCopyable
{
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unlock();

    inline pthread_mutex_t * getMutexLock() { return &_mutex; }

private:
    pthread_mutex_t _mutex;
};

/* RAII 确保锁生命周期结束后一定会被释放*/
class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock & mutex)
    : _mutex(mutex)
    {
        _mutex.lock();
    }

    ~MutexLockGuard()
    {
        _mutex.unlock();
    }

private:
    MutexLock & _mutex;
};

