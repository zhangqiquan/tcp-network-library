// +----------------------------------------------------------------------
// | tcp2 / Acceptor.cc    [ 2022/11/28 16:27 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/Acceptor.h"

namespace zhanshop {
    // 启动子线程
    unsigned int Acceptor::start(function<void(EventLoop* eventLoop)> cb) {
        // 启动主EventLoop
        for(unsigned int i = 0; i < this->_numThreads; ++i){
            auto eventLoop = make_shared<EventLoop>();
            cb(eventLoop.get());
            thread t(bind(&EventLoop::loop, eventLoop.get()));
            t.detach();
            this->_eventLoops.push_back(eventLoop); // 挂载
        }

        return this->_numThreads;
    }

    Acceptor::Acceptor(EventLoop* eventLoop, InetAddress inetAddress, unsigned int numThreads):_inetAddress(inetAddress),_numThreads(numThreads){
        this->_eventLoop = eventLoop;
        //this->_eventLoop->setContext((void*)this);
        this->_acceptSocket = make_unique<Socket>(socket(AF_INET, SOCK_STREAM, 0)); // 非常低级的错误为什么fd会出现close 那是因为没有绑定和监听就给到了epoll
        this->_acceptSocket->setNonBlocking();
        this->_acceptSocket->setReuseAddr();
        this->_acceptSocket->setReusePort();
        this->_acceptSocket->bind(inetAddress);
        this->_acceptSocket->listen();
        // 创建客户端fd 并包装一个fd通道 挂载到epoll
        this->_channel = make_unique<Channel>(eventLoop, this->_acceptSocket->sockfd());
        this->_channel->setReadCallback(bind(&Acceptor::handleRead, this));
        this->_channel->setWriteCallback(bind(&Acceptor::handleWrite, this));
        this->_channel->setCloseCallback(bind(&Acceptor::handleClose, this));
        this->_channel->setErrorCallback(bind(&Acceptor::handleError, this));
        this->_channel->setDestroyCallback(bind(&Acceptor::handleDestroy, this));
        this->_channel->enableRead();

        static auto _acceptObject = this;
        signal(SIGINT, [](int signal){
            _acceptObject->quit();
        });

        // 让所有的子进程退出
        signal(SIGTERM, [](int signal){
            _acceptObject->quit();
        });
    }

    void Acceptor::setConnectionCallback(function<void (EventLoop* eventLoop, InetAddress& address)> cb){
        this->_connectionCallback = cb;
    }

    // 使用子线程来接受新的连接这个思路有些不现实 可以实现但fd的分布一定是不均匀的 经过测试这种思路三错误的必须要用到锁
    void Acceptor::handleRead(){
        struct sockaddr_in clientAddress;
        int clientFd = -1;
        for(;;){
            clientFd = this->_acceptSocket->accept(clientAddress);
            if(clientFd > 0){
                InetAddress address(clientFd, clientAddress);
                this->_connectionCallback(this->getNextLoop(), address); // 这里回调里面需要挂回来
            }else if(clientFd == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)){
                break;
            }else{
                LOG_ERROR("发生异常"+ to_string(clientFd));
                break;
            }
        }

        // 挂载完了去发送通知EventLoop 挂到epoll
        for(auto& it : this->_acceptorTcpConns){
            // 加锁
            it.first->mutexLock()->lock();
            for(auto& it1 : it.second){
                it.first->queuesInLoop(bind(&TcpConnection::connectEstablished, it1)); // 挂载连接
            }
            it.first->mutexLock()->unlock();
            it.first->handleWrite(); // 通知EventLoop 挂载epoll
        }
        this->_acceptorTcpConns.clear();
    }

    EventLoop *Acceptor::getNextLoop() {
        ++this->_next;
        if(this->_next >= this->_numThreads) this->_next = 0;
        return this->_eventLoops[this->_next].get();
    }

    void Acceptor::handleWrite(){
        LOG_DEBUG("Acceptor::handleWrite");
    }

    void Acceptor::handleClose(){
        cout << "handleClose" << endl;
    }

    void Acceptor::handleError(){
        cout << "handleError" << endl;
    }

    void Acceptor::handleDestroy(){

    }

    void Acceptor::acceptor(EventLoop* eventLoop, TcpConnection* tcpConnection){
        this->_acceptorTcpConns[eventLoop].push_back(tcpConnection);
    }

    void Acceptor::connlisten(){
    }

    void Acceptor::quit(){
        // 把所有子线程全部退出
        for(auto& it : this->_eventLoops){
            it->quit(); // 先退出子线程 还是要发送一条消息才行
            it->handleWrite(); // 发送一条消息才行 不然epoll一直都在这里阻塞的
        }
        // 退出主线程
        this->_eventLoop->quit();
        this->_eventLoop->handleWrite();
    }
}