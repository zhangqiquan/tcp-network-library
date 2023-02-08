// +----------------------------------------------------------------------
// | tcp2 / TcpConnection.h    [ 2022/11/28 16:31 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <memory>
#include "Callbacks.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Buffer.h"
#include "Channel.h"
#include "Keepalive.h"

using namespace std;
using namespace zhanshop;
// void TcpConnection::connectEstablished() 添加到EventLoop epoll
// 【这个我先不用它是发消息的不是添加】muduo是在连接初始化void TcpConnection::sendInLoop(const void* data, size_t len) 挂到epoll上去的 我也这么干 这样子就是让所有连接都关联到了 Evoop
namespace zhanshop {
    class EventLoop;
    class TcpConnection {
    protected:
        EventLoop* _eventLoop;
        unique_ptr<Socket> _socket;
        InetAddress _clientAddr;
        unique_ptr<Buffer> _buffer;
        unique_ptr<Channel> _channel;
        weak_ptr<Keepalive> _keepalive;
        void* _context; // 保活弱引用指针 每个Tcp连接都会关联一个保活类 当它被销毁的时候只干一件事情那就是conn->shutdown(); 剩下的让子epoll自己去操作吧
        ConnectionEventCallback _connectionCallback;
        ConnectionEventCallback _messageCallback;
        ConnectionEventCallback _writeCallback;
        ConnectionEventCallback _closeCallback;
    public:
        TcpConnection(EventLoop* eventLoop, InetAddress clientAddr);
        void setConnectionCallback(ConnectionEventCallback cb);
        void setMessageCallback(ConnectionEventCallback cb);
        void setWriteCallback(ConnectionEventCallback cb);
        void setCloseCallback(ConnectionEventCallback cb);
        void connectEstablished(); // 通过通道添加到epoll
        void clientEstablished(); // 通过通道添加到epoll
        void setKeepalive(weak_ptr<Keepalive> keepalive);
        weak_ptr<Keepalive>& keepalive();
        void setContext(void* context);
        void* context();
        void handleRead();
        void handleWrite();
        void handleClose();
        void handleError();
        void handleDestroy();

        EventLoop* eventLoop();
        Socket* socket() const;
        Buffer* buffer();
        InetAddress* clientAddr();
        ~TcpConnection();
    };

}
