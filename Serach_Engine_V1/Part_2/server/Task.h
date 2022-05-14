#pragma once

#include "TcpConnection.h" 
#include "Dictionary.h"
#include <string>
#include <functional>
#include <iostream>
#include <vector>

// 在这里特指 分离出来的 IO_task
using IOTask = std::function<void()>;
using ElementType = std::function<void()>;

class Task
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
public: 
    Task(std::string & msg, const TcpConnectionPtr & pconnection_manager, Dictionary & dictionary)
    : _msg(msg)
    , _pconnection_manager(pconnection_manager)
    , _dictionary(dictionary)
    {
        // std::cout << "Task(std::string & data, TcpConnectionPtr & pconnection_manager)" << std::endl;
    }

    // ~Task()
    // {
    //     std::cout << "~Task()" << std::endl;
    // }

public:
    void taskProcessing();

private:
    bool fuzzyMatching();
    int minEditDistence(std::string word1, std::string word2);

private:
    std::vector<std::string> _recommendWords;
    Dictionary & _dictionary;

private:
    std::string _msg;
    const TcpConnectionPtr & _pconnection_manager;
};


