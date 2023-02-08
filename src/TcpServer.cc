// +----------------------------------------------------------------------
// | tcp2 / MainServer.cc    [ 2022/11/28 16:18 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/TcpServer.h"

namespace zhanshop {
    TcpServer::TcpServer(EventLoop* eventLoop, InetAddress inetAddress, unsigned int numThreads, int keepidle):_eventLoop(eventLoop), _acceptor(make_unique<Acceptor>(eventLoop, inetAddress, numThreads)),_keepaliveBuckets(keepidle){
        for(int i = 0; i < keepidle; i++){
            this->_keepaliveBuckets.push_back(this->_keepaliveBucket);
        }

        // 定时器
        eventLoop->runTick(1000, [this](){
            this->_eventLoop->mutexLock()->lock();
            this->_keepaliveBuckets.push_back(this->_keepaliveBucket); // 推一次
            this->_eventLoop->mutexLock()->unlock();
        });

    }

    void TcpServer::setStartCallback(function<void (TcpServer* tcpServer)> cb){ this->_startCallback = cb;};
    void TcpServer::setStopCallback(function<void (TcpServer* tcpServer)> cb){ this->_stopCallback = cb;};

    void TcpServer::seteventLoopStartCallback(eventLoopEventCallback cb){ this->_eventLoopStartCallback = cb;};
    void TcpServer::seteventLoopStopCallback(eventLoopEventCallback cb){ this->_eventLoopStopCallback = cb;};
    void TcpServer::setConnectionCallback(ConnectionEventCallback cb){
        this->_connectionCallback = cb;
        this->_acceptor->setConnectionCallback(bind(&TcpServer::newConnection, this, _1, _2));
    };
    void TcpServer::setWriteCallback(ConnectionEventCallback cb){ this->_writeCallback = cb;};
    void TcpServer::setMessageCallback(ConnectionEventCallback cb){ this->_messageCallback = cb;};
    void TcpServer::setCloseCallback(ConnectionEventCallback cb){ this->_closeCallback = cb;};

    void TcpServer::workerStart(EventLoop* eventLoop){
        this->_eventLoopStartCallback(eventLoop);
    }

    void TcpServer::workerStop(EventLoop* eventLoop){
        this->_eventLoopStopCallback(eventLoop);
    }

    void TcpServer::newConnection(EventLoop* eventLoop, InetAddress& address){
        auto tcpConnection = make_shared<TcpConnection>(eventLoop, address);
        tcpConnection->setConnectionCallback(bind(&TcpServer::establishConnection, this, _1)); // 连接被挂载后要回调这个方法 connectEstablished后执行
        tcpConnection->setMessageCallback(bind(&TcpServer::messageConnection, this, _1)); // 对于分包的处理丢给上层应用
        tcpConnection->setCloseCallback(bind(&TcpServer::closeConnection, this, _1)); // 这里应该调this本身的方法本身再去调上层回调

        auto keepalive = make_shared<Keepalive>(weak_ptr<TcpConnection>(tcpConnection));
        tcpConnection->setKeepalive(weak_ptr<Keepalive>(keepalive));



        this->_acceptor->acceptor(eventLoop, tcpConnection.get());
        this->_tcpConnections[address.fd()] = tcpConnection; // 塞到全部里面去

        this->_eventLoop->mutexLock()->lock();
        this->_keepaliveBuckets.back().insert(keepalive); // 丢进去包活
        this->_eventLoop->mutexLock()->unlock();

    }

    void TcpServer::establishConnection(TcpConnection* conn){
        //LOG_INFO("连接已经建立"+conn->clientAddr()->ip()+":"+ to_string(conn->clientAddr()->port()));
        this->_connectionCallback(conn);
    }
    void TcpServer::writeConnection(TcpConnection* conn){
        this->_writeCallback(conn);
    }
    // 时间轮上面也需要加锁
    void TcpServer::messageConnection(TcpConnection* conn){
        //cout << "读取到数据了" << conn->buffer()->readData() << endl;
        this->_messageCallback(conn);
        auto weakEntry = conn->keepalive();

        this->_eventLoop->mutexLock()->lock();
        this->_keepaliveBuckets.back().insert(weakEntry.lock());
        this->_eventLoop->mutexLock()->unlock();

        // 可以在上面拿到所有的连接 然后群发 放在主线程有优点也有缺点  优点是方便管理一些 比如群聊分组放在主线程上统一管理
    }

    // 没有用到锁会出bug的明天需要继续研究以下【通过EventLoop::runInLoop， EventLoop::queueInLoop 使用锁加给主线程发送eventFd消息让主线程来销毁】
    void TcpServer::closeConnection(TcpConnection* conn){
        //LOG_DEBUG("TcpServer::closeConnection = 把map上的元素销毁");
        this->_closeCallback(conn);
        // 把map上的元素销毁
        // 这个需要在主线程上完成
        // 这样就算完成 我需要测试两个版本的性能
        this->_eventLoop->queueInLoop(bind(&TcpServer::removeConnectionInLoop, this, conn->socket()->sockfd()));
        this->_eventLoop->handleWrite();
    }
    // TcpConnection* conn 这玩意跨线程了
    void TcpServer::removeConnectionInLoop(int fd){
        this->_tcpConnections.erase(fd);
        ::close(fd);
    }

    void TcpServer::start() {
        this->_acceptor->start([this](EventLoop* eventLoop){
            this->workerStart(eventLoop);
        });
        this->_startCallback(this);
        this->_eventLoop->loop();
    }

    EventLoop* TcpServer::eventLoop(){
        return this->_eventLoop;
    }
    Acceptor* TcpServer::acceptor(){
        return this->_acceptor.get();
    }
    unordered_map<int, shared_ptr<TcpConnection>>* TcpServer::tcpConnections(){
        return &this->_tcpConnections;
    }
    boost::circular_buffer<unordered_set<shared_ptr<Keepalive>>>* TcpServer::keepaliveBuckets(){
        return &this->_keepaliveBuckets;
    }

    TcpServer::~TcpServer(){
        this->_stopCallback(this);
        LOG_WARN("服务器销毁了");
    }
}