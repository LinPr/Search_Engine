// #include "Acceptor.h"
// #include "EventLoop.h"
// #include "Task.h"
// #include "TcpServer.h"
// #include "EventLoop.h"
// #include "ThreadPool.h"
// #include <unistd.h>

#include "Reactor_ThreadPool.h"
#include <iostream>


using std::string;

int main()
{
    size_t threadNum = 4;
    size_t queSize = 10;
    int CacheCapacity = 10;
    string ip = "127.0.0.1";
    unsigned short port = 8888;
    string dictionaryPath = "../../Part_1/StoreDictionary/dictionary";
    string dictionaryIndexPath = "../../Part_1/StoreDictionary/dictionaryIndex";
    
    string webPageLibPath = "../../Part_3/ParsedPages/webPageLib.xml";
    string pagesOffsetMapPath = "../../Part_3/ParsedPages/pagesOffsetMap";
    string invertIndexPath = "../../Part_3/ParsedPages/invertIndex";

    Reactor_ThreadPool reactor_threadPool(
        threadNum, queSize 
        , CacheCapacity
        , ip, port 
        , dictionaryPath, dictionaryIndexPath
        , webPageLibPath, pagesOffsetMapPath, invertIndexPath
    );
    
    reactor_threadPool.reactor_threadPool_start();
    
}

