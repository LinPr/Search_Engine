#pragma once 

#include "TcpServer.h"
#include "ThreadPool.h"
#include "Dictionary.h"
#include "KeyRecommender.h"
#include "WebSource.h"

class Reactor_ThreadPool
{
public:
    Reactor_ThreadPool (
        size_t threadNum, size_t queSize
        , int CacheCapacity
        , const std::string ip, unsigned short port
        , std::string dictionaryPath, std::string dictionaryIndexPath
        , std::string webPageLibPath, std::string pagesOffsetMapPath, std::string invertIndexPath
    );

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
    WebSource & get_webSource()
    {
        return _webSource;
    }
    
private:   
    Dictionary _dictionary;
    KeyRecommender _keyRecommender;
    ThreadPool _threadPool; 
    TcpServer _tcpServer;
    WebSource _webSource;
};

