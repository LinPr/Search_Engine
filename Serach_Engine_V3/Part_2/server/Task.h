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
class Task
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
public: 
    Task(std::string & msg, const TcpConnectionPtr & pconnection_manager, Reactor_ThreadPool * preactor_threadPoll)
    : _msg(msg)
    , _pconnection_manager(pconnection_manager)
    , _preactor_threadPoll(preactor_threadPoll)
    // , _dictionary(dictionary)
    {
        // std::cout << "Task(std::string & data, TcpConnectionPtr & pconnection_manager)" << std::endl;
    }

    // ~Task()
    // {
    //     std::cout << "~Task()" << std::endl;
    // }

public:
    void taskProcessing();

// private:
//     bool fuzzyMatching();
//     int minEditDistence(std::string word1, std::string word2);

// private:
//     std::vector<std::string> _recommendWords;
//     Dictionary & _dictionary;

private:
    Reactor_ThreadPool * _preactor_threadPoll; // 用指针而不是引用是为了方便传 this
private:
    std::string _msg;
    const TcpConnectionPtr & _pconnection_manager;
};


class TimerTask
{
public:

private:
    
};


