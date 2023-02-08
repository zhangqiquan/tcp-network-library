// +----------------------------------------------------------------------
// | tcp2 / MainServer.h    [ 2022/11/28 16:18 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <memory>
#include <functional>
#include <boost/circular_buffer.hpp>
#include <unordered_set>
#include <unordered_map>
#include "Callbacks.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "Keepalive.h"
#include "Timer.h"

using namespace std;

namespace zhanshop {

    class TcpServer {
    protected:
        EventLoop* _eventLoop;
        unique_ptr<Acceptor> _acceptor; // 对应的是每一个socket
        unordered_map<int, shared_ptr<TcpConnection>> _tcpConnections; // 所有的连接都挂在主线程上 (对于群发来说 走出线程)
        boost::circular_buffer<unordered_set<shared_ptr<Keepalive>>> _keepaliveBuckets;
        unordered_set<shared_ptr<Keepalive>> _keepaliveBucket;
        //unique_ptr<Timer> _timer; // 对应每个Crontab

        // 主线程启动 停止 2
        function<void (TcpServer* tcpServer)> _startCallback;
        function<void (TcpServer* tcpServer)> _stopCallback;
        // 子线程启动停止 2
        eventLoopEventCallback _eventLoopStartCallback;
        eventLoopEventCallback _eventLoopStopCallback;
        // 子线程收到连接 消息 关闭 3
        ConnectionEventCallback _connectionCallback;
        ConnectionEventCallback _writeCallback;
        ConnectionEventCallback _messageCallback;
        ConnectionEventCallback _closeCallback;
    public:
        TcpServer(EventLoop* eventLoop, InetAddress inetAddress, unsigned int numThreads, int keepidle = 5);

        void setStartCallback(function<void (TcpServer* tcpServer)> cb);
        void setStopCallback(function<void (TcpServer* tcpServer)> cb);

        void seteventLoopStartCallback(eventLoopEventCallback cb);
        void seteventLoopStopCallback(eventLoopEventCallback cb);

        void setConnectionCallback(ConnectionEventCallback cb);
        void setWriteCallback(ConnectionEventCallback cb);
        void setMessageCallback(ConnectionEventCallback cb);
        void setCloseCallback(ConnectionEventCallback cb);

        void workerStart(EventLoop* eventLoop);
        void workerStop(EventLoop* eventLoop);
        void newConnection(EventLoop* eventLoop, InetAddress& address);
        void establishConnection(TcpConnection* conn);
        void writeConnection(TcpConnection* conn);
        void messageConnection(TcpConnection* conn); // 如果三广播的话只能在这里处理
        void sendConnection(TcpConnection* conn);
        void closeConnection(TcpConnection* conn);
        void removeConnectionInLoop(int fd);

        void start();

        /**
         * EventLoop* _eventLoop;
        unique_ptr<Acceptor> _acceptor; // 对应的是每一个socket
        unordered_map<int, shared_ptr<TcpConnection>> _tcpConnections; // 所有的连接都挂在主线程上 (对于群发来说 走出线程)
        boost::circular_buffer<unordered_set<shared_ptr<Keepalive>>> _keepaliveBuckets;
         */
        EventLoop* eventLoop();
        Acceptor* acceptor();
        unordered_map<int, shared_ptr<TcpConnection>>* tcpConnections();
        boost::circular_buffer<unordered_set<shared_ptr<Keepalive>>>* keepaliveBuckets();
        ~TcpServer();


    };

}
