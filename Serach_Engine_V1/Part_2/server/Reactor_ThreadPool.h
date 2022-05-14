#pragma once 

#include "TcpServer.h"
#include "ThreadPool.h"
#include "Dictionary.h"

class Reactor_ThreadPool
{
public:
    Reactor_ThreadPool (size_t threadNum, size_t queSize, 
                        const std::string ip, unsigned short port,
                        std::string dictionaryPath, std::string dictionaryIndexPath);

    ~Reactor_ThreadPool();

    
    void Establish(const TcpConnectionPtr & pconnection_manager);
    void Message(const TcpConnectionPtr & pconnection_manager);
    void Close(const TcpConnectionPtr & pconnection_manager);

    void reactor_threadPool_start();
    void reactor_threadPool_stop();
    
private:   
    Dictionary _dictionary;
    ThreadPool _threadPool; 
    TcpServer _tcpServer;
    
    
};

