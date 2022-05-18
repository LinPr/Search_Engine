#pragma once 

#include "Acceptor.h"
#include "TcpConnection.h"
#include "Task.h"
#include "MutexLock.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function< void(const TcpConnectionPtr &) >;


class EventLoop
{
public:
    EventLoop(Acceptor & acceptor);
    ~EventLoop();

    void startLoop();
    void stopLoop();

public: 
    void setEstablishCallback(const TcpConnectionCallback & cb);
    void setMessageCallback(const TcpConnectionCallback & cb);
    void setCloseCallback(const TcpConnectionCallback & cb);

private:
    void EpollWait();
    void handleNewconnection();
    void handleMessage(int fd);

    int createEpollFD();
    void addEpollFD(int fd);
    void delEpollFD(int fd);

private:
    TcpConnectionCallback _onEstablish_cb;
    TcpConnectionCallback _onMessage_cb;
    TcpConnectionCallback _onClose_cb;

private:
    int _epfd;
    Acceptor & _acceptor;
    bool _isLooping;
    std::vector<struct epoll_event> _readyList;
    std::map<int, TcpConnectionPtr> _connectionsManager;


/* 引入线程池， 并将CPU密集任务和IO密集任务分离 */

public:
    int  evnetfdCreate();
    void eventfdWait();
    void eventfdNotify();

    // IOTask在线程池的线程中被注册
    // void addIOtask2List(const IOTask & io_task);
    void addIOtask2List(const IOTask & io_task);
    // IOTask在主线程中被执行
    void excuteIOtask();
private:
    int _eventfd;
    std::vector<IOTask> _IOtaskList; // _taskList 在这实际都是回调函数
    MutexLock _mutex;

};
