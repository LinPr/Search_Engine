#include "Reactor_ThreadPool.h"
#include <unistd.h>


Reactor_ThreadPool::Reactor_ThreadPool(size_t threadNum, size_t queSize, int CacheCapacity, const std::string ip, unsigned short port, std::string dictionaryPath, std::string dictionaryIndexPath)
: _threadPool(threadNum, queSize, CacheCapacity)
, _tcpServer(ip, port)
, _dictionary(dictionaryPath, dictionaryIndexPath)
, _keyRecommender(_dictionary, _threadPool.get_cachesManager())
{
    
    std::cout << "Reactor_ThreadPool(size_t threadNum, size_t queSize, const std::string ip, unsigned short port)" << std::endl;
}

Reactor_ThreadPool::~Reactor_ThreadPool()
{
    reactor_threadPool_stop();
    std::cout << "~Reactor_ThreadPool()" << std::endl;
}


void Reactor_ThreadPool::reactor_threadPool_start()
{   
    /* 1. 把字典加载进内存 */
    _dictionary.loadDictionary();

    /* 2. 启动线程池 */
    _threadPool.startPool();

    /* 3. 设置回调函数 */
    _tcpServer.registerAllCallback(
        std::bind(&Reactor_ThreadPool::Establish, this, std::placeholders::_1),  
        std::bind(&Reactor_ThreadPool::Message  , this, std::placeholders::_1),
        std::bind(&Reactor_ThreadPool::Close    , this, std::placeholders::_1) );
    
    /* 4. 启动事件循环 */
    _tcpServer.tcpServerStart();
}

void Reactor_ThreadPool::reactor_threadPool_stop()
{
    _threadPool.stopPool();
    _tcpServer.tcpServerStop();
}



void Reactor_ThreadPool::Establish(const TcpConnectionPtr & pconnection_manager)
{
    std::cout << pconnection_manager->connectionInfo() 
        << "    is established !!" << std::endl;
}


void Reactor_ThreadPool::Message(const TcpConnectionPtr & pconnection_manager)
{
    /* 1. 主线程接收客户端数据 */
    std::string msg = pconnection_manager->receiveData();
    std::cout << "recv msg from client : " << msg << std::endl;
   
    /* 2. 把收到的数据打包一下，真正业务逻辑处理丢给线程池 */
    Task task(msg, pconnection_manager, this);

    /* 为什么这里要用task， 而不是传 Task 的地址，答曰：task作用域，必须传拷贝*/ 
    _threadPool.addTask(std::bind(&Task::taskProcessing, task));

    
    // pconnection_manager->sendData(msg); 发送数据不在这里了
}

void Reactor_ThreadPool::Close(const TcpConnectionPtr & pconnection_manager)
{
    std::cout << pconnection_manager->connectionInfo() 
        << "    is closed  !!" << std::endl;
}