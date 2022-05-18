#include "EventLoop.h"
#include <unistd.h>
#include <iostream>
#include <sys/eventfd.h>


EventLoop::EventLoop(Acceptor & acceptor)
: _epfd(createEpollFD())
, _acceptor(acceptor)
, _isLooping(false)
, _readyList(1024)
, _connectionsManager()
, _eventfd(evnetfdCreate())
{
    addEpollFD(acceptor.listenfd());
    addEpollFD(_eventfd);
    std::cout << "EventLoop(Acceptor & acceptor)" << std::endl;
}

EventLoop::~EventLoop()
{
    if(_epfd) { ::close(_epfd); }
    std::cout << "~EventLoop()" << std::endl;
}

void EventLoop::startLoop()
{
    _isLooping = true;
    while(_isLooping)
    {
        EpollWait();
    }
}
void EventLoop::stopLoop()
{   
    _isLooping = false;
}

void EventLoop::setEstablishCallback(const TcpConnectionCallback & cb)
{
    _onEstablish_cb = cb;
}

void EventLoop::setMessageCallback(const TcpConnectionCallback & cb)
{
    _onMessage_cb = cb;
}

void EventLoop::setCloseCallback(const TcpConnectionCallback & cb)
{
    _onClose_cb = cb;
}

void EventLoop::EpollWait()
{
    int readyNum = ::epoll_wait(_epfd, &*_readyList.begin(), _readyList.size(), 5000);
    std::cout << "readyNum = " << readyNum << std::endl;

    while(-1 == readyNum && errno == EINTR)
    { // 中断恢复
        readyNum = ::epoll_wait(_epfd, &_readyList[0], _readyList.size(), 5000);
    }

    if(-1 == readyNum) {perror("epoll_wait"); return; }
    else if(0 == readyNum) { /* printf("epoll_wait timeout\n "); */}
    else 
    {
        if(_readyList.size() == readyNum) {_readyList.resize(2 * readyNum);}

        for(int i = 0; i < readyNum; ++i)
        {
            /* 提取 */
            int fd = _readyList[i].data.fd;
            uint32_t event = _readyList[i].events;

            if(fd == _acceptor.listenfd() && (event & EPOLLIN)) 
            { // 新的连接请求
                std::cout << " handleNewconnection();" << std::endl;
                handleNewconnection();
            }
            else if(fd == _eventfd && (event & EPOLLIN))
            {  //为什么知道是这个分支还要read一下_evtfd？答曰：为了清空_enentfd
                eventfdWait();
                excuteIOtask();
            }
            else if(event & EPOLLIN) // 注意else if 的判断顺序具有短路原则
            { // 主线程中处理IO读事件
                handleMessage(fd);
            }
            else
            { // 其他类型事件

            }
        }
    }    
}

void EventLoop::handleNewconnection()
{
    int connfd = _acceptor.accept();
    addEpollFD(connfd); // 加入红黑树
    TcpConnectionPtr connection_manager(new TcpConnection(connfd, this)); //添加TCP连接管理


    /* 回调函数的注册 1 2 3*/
    connection_manager->setEstablishCallback(_onEstablish_cb);
    connection_manager->setMessageCallback(_onMessage_cb);
    connection_manager->setCloseCallback(_onClose_cb);
    
    // 加入TCP连接池
    _connectionsManager.emplace(connfd, connection_manager);

    
    /* 执行回调函数 1 */
    connection_manager->handleEstablishCallback();
}

void EventLoop::handleMessage(int fd)
{
    auto it = _connectionsManager.find(fd);
    if(it != _connectionsManager.end())
    {   
        // std::cout << "handleMessage(fd);" << std::endl;
        if(it->second->isPeerClosed())
        {
            // it->second->handleMessageCallback();   
            delEpollFD(fd);
            _connectionsManager.erase(it); // 关闭TCP连接，并释放所有资源
        }
        else
        {
            it->second->handleMessageCallback();
        }
        
    }
}

int EventLoop::createEpollFD()
{
   /* the size argument is ignored but must be greater than zero;  */
    int epfd = epoll_create(1);
    if(epfd == -1) {perror("epoll_create"); return -1;}
    
    return epfd;
}

void EventLoop::addEpollFD(int fd)
{
    struct  epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event);
    if(ret == -1) {perror("epoll_ctl"); return;}
}

void EventLoop::delEpollFD(int fd)
{
    struct  epoll_event event;
    // event.data.fd = fd;
    // event.events = EPOLLIN;
    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr);
    if(ret == -1) {perror("epoll_ctl"); return;}
}



int EventLoop::evnetfdCreate()
{
    int fd = eventfd(0, 0);
    if(-1 == fd) { perror("eventfd"); exit(-1);}
    return fd;
}

void EventLoop::eventfdWait()
{
    uint64_t tmp = 0;
    int ret = read(_eventfd, &tmp, sizeof(tmp));
    if(ret != sizeof(tmp)) { perror("read eventfd"); return; }
}

void EventLoop::eventfdNotify()
{
    uint64_t tmp = 1;
    int ret = write(_eventfd, &tmp, sizeof(tmp));
    if(ret != sizeof(tmp)) { perror("write eventfd"); return; }
}

// void EventLoop::addIOtask2List(const IOTask & io_task)
void EventLoop::addIOtask2List(const IOTask & io_task)
{   
// 因为 _IOtaskList 是个共享资源，所以需要上锁
    {
        MutexLockGuard autoLock(_mutex);
        _IOtaskList.push_back(io_task);
    }

    eventfdNotify();
}

void EventLoop::excuteIOtask()
{
    /* 首先将 _IOtaskList.中，已注册待执行的 IO 任务取出 */
    std::vector<IOTask> tmp_IOtasks;

    {
        MutexLockGuard autoLock(_mutex);
        tmp_IOtasks.swap(_IOtaskList); // IO读事件队列被换成空队列（巧妙！）
    }

    for(auto & task : tmp_IOtasks)
    {
        std::cout << "send to client" << std::endl;
        task();
    }
}