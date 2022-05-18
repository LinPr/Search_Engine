#include "Reactor_ThreadPool.h"
#include "ProtocolUnit.h"
#include <unistd.h>


enum{ KEY_RECOMMAND = 1, WEBPAGE_SEARCH};

Reactor_ThreadPool::Reactor_ThreadPool(
    size_t threadNum, size_t queSize
    , int CacheCapacity
    , const std::string ip, unsigned short port
    , std::string dictionaryPath, std::string dictionaryIndexPath
    , std::string webPageLibPath, std::string pagesOffsetMapPath, std::string invertIndexPath
)
: _threadPool(threadNum, queSize, CacheCapacity)
, _tcpServer(ip, port)
, _dictionary(dictionaryPath, dictionaryIndexPath)
, _keyRecommender(_dictionary, _threadPool.get_cachesManager())
, _webSource(webPageLibPath, pagesOffsetMapPath, invertIndexPath)
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
    /* 1. 把字典加载进内存 */  /* 加载网页，但我直接在构造函数加载好了 */
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
   
    /* 2. 解析数据, 根据协议将数据打包成不同的任务 */
    ProtocolUnit protocolUnit(msg);
    if(KEY_RECOMMAND == protocolUnit.get_protocalID())
    {
        /* 3. 创建单词推荐任务，真正业务逻辑处理丢给线程池 */
        Task task(protocolUnit.get_rawData(), pconnection_manager, this);
        /* 为什么这里要用task， 而不是传 Task 的地址，答曰：task作用域，必须传拷贝*/ 
        _threadPool.addTask(std::bind(&Task::taskProcessing, task));

    }
    else if(WEBPAGE_SEARCH == protocolUnit.get_protocalID())
    {
        /* 创建网页查询任务，丢给线程城 */
        SearchWebPageTask searchWebPageTask(protocolUnit.get_rawData(), pconnection_manager, this);
        _threadPool.addTask(std::bind(&SearchWebPageTask::SearchWebPageTaskProcessing, searchWebPageTask));
    }
    else
    {

    }



}

void Reactor_ThreadPool::Close(const TcpConnectionPtr & pconnection_manager)
{
    std::cout << pconnection_manager->connectionInfo() 
        << "    is closed  !!" << std::endl;
}