// +----------------------------------------------------------------------
// | tcp2 / Acceptor.h    [ 2022/11/28 16:27 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <signal.h>
#include <memory>
#include <map>
#include <vector>
#include <unordered_set>
#include <thread>
#include "Callbacks.h"
#include "Socket.h"
#include "InetAddress.h"
#include "EventLoop.h"
#include "Dispatcher.h"
#include "Channel.h"
#include "TcpConnection.h"

using namespace std;

namespace zhanshop {
    class Acceptor {
    protected:
        EventLoop* _eventLoop;
        unique_ptr<Socket> _acceptSocket;
        InetAddress _inetAddress;
        unsigned int _numThreads = 1;
        unique_ptr<Channel> _channel;
        vector<shared_ptr<EventLoop>> _eventLoops;
        map<EventLoop*, vector<TcpConnection*>> _acceptorTcpConns;
        function<void (EventLoop* eventLoop, InetAddress& address)> _connectionCallback;
        unsigned int _next = 0;
        void loop();
        void quit();
    public:
        Acceptor(EventLoop* eventLoop, InetAddress inetAddress, unsigned int numThreads);
        void setConnectionCallback(function<void (EventLoop* eventLoop, InetAddress& address)> cb);
        void acceptor(EventLoop*, TcpConnection*);
        void handleRead();
        void handleWrite();
        void handleClose();
        void handleError();
        void handleDestroy();
        unsigned int start(function<void(EventLoop* eventLoop)> cb);
        void connlisten();
        EventLoop* getNextLoop();
    };

}
