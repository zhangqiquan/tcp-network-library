// +----------------------------------------------------------------------
// | zhanshop-tcp2 / index.cc    [ 2022/12/1 上午10:16 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../../include/Error.h"
#include "../../include/EventLoop.h"
#include "../../include/TcpServer.h"

using namespace zhanshop;

TcpServer* tcpServer;

void onStop(TcpServer* _tcpServer){
}

void onWorkerStart(EventLoop* eventLoop){
    // 做请求
}

void onWorkerStop(EventLoop* eventLoop){
}

void onConnection(TcpConnection* conn){
}

void onWrite(TcpConnection* conn){
    string msg = "123";
    conn->socket()->write(msg.c_str(), msg.length());
}
void onMessage(TcpConnection* conn){
    //LOG_DEBUG("收到消息："+conn->buffer()->readData());
    conn->socket()->close();
    tcpServer->eventLoop()->queueInLoop(bind(&TcpServer::removeConnectionInLoop, tcpServer, conn->socket()->sockfd()));
    tcpServer->eventLoop()->handleWrite();
}

void onClose(TcpConnection* conn){
    tcpServer->eventLoop()->queueInLoop(bind(&TcpServer::removeConnectionInLoop, tcpServer, conn->socket()->sockfd()));
    tcpServer->eventLoop()->handleWrite();
}

void onStart(TcpServer* _tcpServer){
    tcpServer = _tcpServer;
    LOG_DEBUG("==onStart==");
    map<EventLoop*, vector<TcpConnection*>> acceptorTcpConns;

    for(int i = 0; i < 10; ++i){
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        int flag = fcntl(fd, F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(fd, F_SETFL, flag);

        InetAddress address("114.114.114.114", 6211);
        address.setFd(fd);
        auto eventLoop = _tcpServer->acceptor()->getNextLoop();
        auto tcpConnection = make_shared<TcpConnection>(eventLoop, address);
        tcpConnection->setConnectionCallback(onConnection); // 连接被挂载后要回调这个方法 connectEstablished后执行
        tcpConnection->setWriteCallback(onWrite);
        tcpConnection->setMessageCallback(onMessage); // 对于分包的处理丢给上层应用
        tcpConnection->setCloseCallback(onClose); // 这里应该调this本身的方法本身再去调上层回调

        auto keepalive = make_shared<Keepalive>(weak_ptr<TcpConnection>(tcpConnection));
        tcpConnection->setKeepalive(weak_ptr<Keepalive>(keepalive));
        _tcpServer->keepaliveBuckets()->back().insert(keepalive); // 丢进去包活
        acceptorTcpConns[eventLoop].push_back(tcpConnection.get());
        _tcpServer->tcpConnections()->insert({fd, tcpConnection}); // 塞到全部里面去
    }

    // 挂载完了去发送通知EventLoop 挂到epoll
    for(auto& it : acceptorTcpConns){
        // 加锁
        it.first->mutexLock()->lock();
        for(auto& it1 : it.second){
            it.first->queuesInLoop(bind(&TcpConnection::clientEstablished, it1)); // 挂载连接
        }
        it.first->mutexLock()->unlock();
        it.first->handleWrite(); // 通知EventLoop 挂载epoll
    }
    acceptorTcpConns.clear();

    _tcpServer->eventLoop()->runTick(1000, [_tcpServer](){
        if(_tcpServer->tcpConnections()->size() == 0){
            LOG_INFO("执行完毕");
            exit(-1);
        }
    });
}



int main(){
    EventLoop eventLoop;
    Error::listen();

    auto client = make_unique<TcpServer>(&eventLoop, InetAddress("127.0.0.1", 4444), 4, 5);
    client->setStartCallback(onStart);
    client->setStopCallback(onStop);
    client->seteventLoopStartCallback(onWorkerStart);
    client->seteventLoopStopCallback(onWorkerStop);
    client->setConnectionCallback(onConnection);
    client->setWriteCallback(onWrite);
    client->setMessageCallback(onMessage);
    client->setCloseCallback(onClose);

    client->start();

    return 0;
}