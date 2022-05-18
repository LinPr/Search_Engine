#pragma once 

#include "TcpServer.h"
#include "ThreadPool.h"
#include "Dictionary.h"
#include "KeyRecommender.h"

class Reactor_ThreadPool
{
public:
    Reactor_ThreadPool (size_t threadNum, size_t queSize, int CacheCapacity,
                        const std::string ip, unsigned short port,
                        std::string dictionaryPath, std::string dictionaryIndexPath);

    ~Reactor_ThreadPool();

    
    void Establish(const TcpConnectionPtr & pconnection_manager);
    void Message(const TcpConnectionPtr & pconnection_manager);
    void Close(const TcpConnectionPtr & pconnection_manager);

    void reactor_threadPool_start();
    void reactor_threadPool_stop();

public:
    KeyRecommender & getKeyRecommender()
    {
        return _keyRecommender;
    }
    
private:   
    Dictionary _dictionary;
    KeyRecommender _keyRecommender;
    ThreadPool _threadPool; 
    TcpServer _tcpServer;
};

