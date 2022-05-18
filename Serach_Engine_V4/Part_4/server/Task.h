#pragma once

#include "TcpConnection.h" 
// #include "Dictionary.h"

#include <string>
#include <functional>
#include <iostream>
#include <vector>

// 在这里特指 分离出来的 IO_task
using IOTask = std::function<void()>;
using ElementType = std::function<void()>;

class Reactor_ThreadPool;

/* 1. 关键字推荐任务 */
class Task
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
public: 
    Task(std::string & msg, const TcpConnectionPtr & pconnection_manager, Reactor_ThreadPool * preactor_threadPoll)
    : _msg(msg)
    , _pconnection_manager(pconnection_manager)
    , _preactor_threadPoll(preactor_threadPoll)
    {
       
    }

 

public:
    void taskProcessing();

private:
    Reactor_ThreadPool * _preactor_threadPoll; // 用指针而不是引用是为了方便传 this
private:
    std::string _msg;
    const TcpConnectionPtr & _pconnection_manager;
};


/* 搜索网页任务 */
class SearchWebPageTask
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
public:
    SearchWebPageTask(std::string & msg, const TcpConnectionPtr & pconnection_manager, Reactor_ThreadPool * preactor_threadPoll)
    : _msg(msg)
    , _pconnection_manager(pconnection_manager)
    , _preactor_threadPoll(preactor_threadPoll)
    {

    }

    void SearchWebPageTaskProcessing();

private:
    std::string _msg;
    Reactor_ThreadPool * _preactor_threadPoll;
    const TcpConnectionPtr & _pconnection_manager;
};



/* 定时器任务 */
class TimerTask
{
public:

private:
    
};


