#include "TimerTask.h"
#include "Thread.h"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

void timerTaskPocess()
{
    cout << "The timerTaskPocess is running" << endl;
}

void test()
{
    TimerTask timerTask(3, 3, timerTaskPocess);

    Thread th(std::bind(&TimerTask::startTimer, &timerTask));//子线程
    th.start();

    pause();


    timerTask.stopTimer();
    th.join();
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

