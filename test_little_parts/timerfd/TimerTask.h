#pragma once 
#include <functional>


class TimerTask
{
    using TimerTaskCallback = std::function<void()>;
public:
    TimerTask(int initsec, int periodicsec, const TimerTaskCallback & timerTaskCallback);
    ~TimerTask();

    void startTimer();
    void stopTimer();

    void timerfdWait();

private:
    int createTimerFd();
    void setTimerFd(int init, int periodic);

private:
    int _timerfd;
    int _initsec;
    int _periodicsec;
    TimerTaskCallback _timerTaskCallback;
    bool _isStarted;
};


