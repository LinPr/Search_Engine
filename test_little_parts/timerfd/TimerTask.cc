#include "TimerTask.h"
#include <unistd.h>
#include <sys/timerfd.h>
#include <poll.h>
#include <iostream>

TimerTask::TimerTask(int initsec, int periodicsec, const TimerTaskCallback & timerTaskCallback)
: _timerfd(createTimerFd())
, _initsec(initsec)
, _periodicsec(periodicsec)
, _timerTaskCallback(timerTaskCallback)
, _isStarted(false)
{
    std::cout << "TimerTask(int initsec, int periodicsec, TimerTaskCallback &&cb)" << std::endl;
}

TimerTask::~TimerTask()
{
    if(_timerfd)
    {
        setTimerFd(0, 0);
        close(_timerfd);
    }
}

void TimerTask::startTimer()
{
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = _timerfd;
    

    setTimerFd(_initsec, _periodicsec);

    _isStarted = true;
    while(_isStarted)
    {
        int timerReady = poll(&pfd, 1, 5000);
        printf("timerReady : %d\n", timerReady);

        if(-1 == timerReady && errno == EINTR)
        {
            continue;
        }
        else if(-1 == timerReady)
        {
            perror("poll -1 == timerReady");
            return;
        }
        else if(0 == timerReady)
        {
            printf("poll timeout\n");
        }
        else
        {
            if(pfd.revents & POLLIN)
            {
                timerfdWait();//处理读事件
                if(_timerTaskCallback)
                {
                    _timerTaskCallback();//执行任务
                }
            }
        }
    }
}

void TimerTask::stopTimer()
{
    _isStarted = false;
}


void TimerTask::timerfdWait()
{
    uint64_t tmp = 0;
    int ret = read(_timerfd, &tmp, sizeof(tmp));
    if(ret != sizeof(tmp)) { perror("read timerfd"); return; }
}


int TimerTask::createTimerFd()
{
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if(-1 == fd) { perror("timerfd_create"); exit(-1); }

    return fd;
}


void TimerTask::setTimerFd(int init, int periodic)
{
    struct itimerspec value;
    value.it_value.tv_sec = init; /* Initial expiration */
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = periodic; /* Interval for periodic timer */
    value.it_interval.tv_nsec = 0;

    int ret = timerfd_settime(_timerfd, 0, &value, nullptr);
    if(ret == -1) { perror("timerfd_settime"); exit(-1);}
}
