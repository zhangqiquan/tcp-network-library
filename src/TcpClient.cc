// +----------------------------------------------------------------------
// | tcp2 / MainServer.cc    [ 2022/11/28 16:18 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/TcpClient.h"

namespace zhanshop {
    TcpClient::TcpClient(EventLoop* eventLoop, InetAddress inetAddress, unsigned int numThreads, int keepidle):_eventLoop(eventLoop), _acceptor(make_unique<Acceptor>(eventLoop, inetAddress, numThreads)),_keepaliveBuckets(keepidle){
        for(int i = 0; i < keepidle; i++){
            this->_keepaliveBuckets.push_back(this->_keepaliveBucket);
        }

        // 定时器
        eventLoop->runTick(1000, [this](){
            this->_keepaliveBuckets.push_back(this->_keepaliveBucket); // 推一次
        });

    }

    void TcpClient::setStartCallback(eventLoopEventCallback cb){ this->_eventLoopStartCallback = cb;};
    void TcpClient::setStopCallback(eventLoopEventCallback cb){ this->_eventLoopStopCallback = cb;};

    void TcpClient::seteventLoopStartCallback(eventLoopEventCallback cb){ this->_eventLoopStartCallback = cb;};
    void TcpClient::seteventLoopStopCallback(eventLoopEventCallback cb){ this->_eventLoopStopCallback = cb;};
    void TcpClient::setConnectionCallback(ConnectionEventCallback cb){
        this->_connectionCallback = cb;
        this->_acceptor->setConnectionCallback(bind(&TcpClient::newConnection, this, _1, _2));
    };
    void TcpClient::setMessageCallback(ConnectionEventCallback cb){ this->_messageCallback = cb;};
    void TcpClient::setCloseCallback(ConnectionEventCallback cb){ this->_closeCallback = cb;};

    void TcpClient::workerStart(EventLoop* eventLoop){
        this->_eventLoopStartCallback(eventLoop);

        vector<TcpConnection*> conns;

        for(int i = 0; i < 2; ++i){
            int fd = socket(AF_INET, SOCK_STREAM, 0);
            int flag = fcntl(fd, F_GETFL);
            flag |= O_NONBLOCK;
            fcntl(fd, F_SETFL, flag);

            InetAddress address("43.130.12.177", 80);
            address.setFd(fd);

            auto tcpConnection = make_shared<TcpConnection>(eventLoop, address);
            tcpConnection->setConnectionCallback(bind(&TcpClient::establishConnection, this, _1)); // 连接被挂载后要回调这个方法 connectEstablished后执行
            tcpConnection->setWriteCallback(bind(&TcpClient::writeConnection, this, _1));
            tcpConnection->setMessageCallback(bind(&TcpClient::messageConnection, this, _1)); // 对于分包的处理丢给上层应用
            tcpConnection->setCloseCallback(bind(&TcpClient::closeConnection, this, _1)); // 这里应该调this本身的方法本身再去调上层回调

            auto keepalive = make_shared<Keepalive>(weak_ptr<TcpConnection>(tcpConnection));
            tcpConnection->setKeepalive(weak_ptr<Keepalive>(keepalive));
            this->_keepaliveBuckets.back().insert(keepalive); // 丢进去包活
            conns.push_back(tcpConnection.get());
            this->_tcpConnections[fd] = tcpConnection; // 塞到全部里面去
        }

        eventLoop->mutexLock()->lock();

        for(auto& it : conns){
            eventLoop->queuesInLoop(bind(&TcpConnection::clientEstablished, it)); // 挂载连接
        }

        eventLoop->mutexLock()->unlock();
        eventLoop->handleWrite();
    }

    void TcpClient::workerStop(EventLoop* eventLoop){
        this->_eventLoopStopCallback(eventLoop);
    }

    void TcpClient::newConnection(EventLoop* eventLoop, InetAddress& address){
        auto tcpConnection = make_shared<TcpConnection>(eventLoop, address);
        tcpConnection->setConnectionCallback(bind(&TcpClient::establishConnection, this, _1)); // 连接被挂载后要回调这个方法 connectEstablished后执行
        tcpConnection->setMessageCallback(bind(&TcpClient::messageConnection, this, _1)); // 对于分包的处理丢给上层应用
        tcpConnection->setCloseCallback(bind(&TcpClient::closeConnection, this, _1)); // 这里应该调this本身的方法本身再去调上层回调

        auto keepalive = make_shared<Keepalive>(weak_ptr<TcpConnection>(tcpConnection));
        tcpConnection->setKeepalive(weak_ptr<Keepalive>(keepalive));
        this->_keepaliveBuckets.back().insert(keepalive); // 丢进去包活
        this->_acceptor->acceptor(eventLoop, tcpConnection.get());
        this->_tcpConnections[address.fd()] = tcpConnection; // 塞到全部里面去
    }

    void TcpClient::establishConnection(TcpConnection* conn){
        LOG_INFO("连接已经建立"+conn->clientAddr()->ip()+":"+ to_string(conn->clientAddr()->port()));
    }
    void TcpClient::writeConnection(TcpConnection* conn){
        string msg = "123";
        conn->socket()->write(msg.c_str(), msg.length());
    }

    void TcpClient::messageConnection(TcpConnection* conn){
        //cout << "读取到数据了" << conn->buffer()->readData() << endl;
        this->_messageCallback(conn);
        // 可以在上面拿到所有的连接 然后群发 放在主线程有优点也有缺点  优点是方便管理一些 比如群聊分组放在主线程上统一管理
        //this->_acceptor->
    }

    // 没有用到锁会出bug的明天需要继续研究以下【通过EventLoop::runInLoop， EventLoop::queueInLoop 使用锁加给主线程发送eventFd消息让主线程来销毁】
    void TcpClient::closeConnection(TcpConnection* conn){
        //LOG_DEBUG("TcpClient::closeConnection = 把map上的元素销毁");
        this->_closeCallback(conn);
        // 把map上的元素销毁
        // 这个需要在主线程上完成
        // 这样就算完成 我需要测试两个版本的性能
        this->_eventLoop->queueInLoop(bind(&TcpClient::removeConnectionInLoop, this, conn));
        this->_eventLoop->handleWrite();
    }

    void TcpClient::removeConnectionInLoop(TcpConnection* conn){
        this->_tcpConnections.erase(conn->socket()->sockfd());
    }

    void TcpClient::start() {
        vector<EventLoop*> eventLoops;
        for(unsigned int i = 0; i < this->_acceptor->start([](EventLoop* eventLoop){}); ++i){
            auto loop = this->_acceptor->getNextLoop();
            eventLoops.push_back(loop);
            this->workerStart(loop);
        }
        this->_startCallback(this->_eventLoop, eventLoops);
        this->_eventLoop->loop();
    }

    TcpClient::~TcpClient(){
        vector<EventLoop*> eventLoops;
        for(unsigned int i = 0; i < this->_acceptor->start([](EventLoop* eventLoop){}); ++i){
            auto loop = this->_acceptor->getNextLoop();
            eventLoops.push_back(loop);
            this->workerStop(loop);
        }
        this->_stopCallback(this->_eventLoop, eventLoops);
        LOG_WARN("服务器销毁了");
    }
}