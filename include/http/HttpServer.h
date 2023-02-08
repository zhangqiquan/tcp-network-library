// +----------------------------------------------------------------------
// | zhanshop / echo.hpp    [ 2022/12/1 上午10:21 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include "../InetAddress.h"
#include "../TcpServer.h"
#include "../TcpConnection.h"
#include "../EventLoop.h"
#include "HttpService.h"
#include "HttpContext.h"
#include "HttpResponse.h"

using namespace placeholders;
using namespace zhanshop;

namespace zhanshop {
    // 这次给上默认值
    class HttpServer {
    protected:
        function<void (TcpServer* tcpServer)> _startCallback;
        function<void (TcpServer* tcpServer)> _stopCallback;
        // 子线程启动停止 2
        // function<void (HttpService* httpService)>
        function<void (HttpService* httpService)> _workerStartCallback;
        function<void (HttpService* httpService)> _workerStopCallback;
        // 子线程收到连接 消息 关闭 3
        ConnectionEventCallback _connectionCallback;
        ConnectionEventCallback _writeCallback;
        function<void (TcpConnection* connection, HttpRequest* httpRequest, HttpResponse* httpResponse)> _messageCallback;
        ConnectionEventCallback _closeCallback;

        unique_ptr<TcpServer> _tcpServer;

        void onStart(TcpServer* _tcpServer);

        void onStop(TcpServer* _tcpServer);

        void onWorkerStart(EventLoop* eventLoop);

        void onWorkerStop(EventLoop* eventLoop);

        void onConnection(TcpConnection* conn);

        void onMessage(TcpConnection* conn);

        void onClose(TcpConnection* conn);

    public:
        HttpServer(EventLoop* eventLoop, string ip, int port, unsigned int threadNum);

        void setStartCallback(function<void (TcpServer* tcpServer)> cb);
        void setStopCallback(function<void (TcpServer* tcpServer)> cb);
        void setWorkerStartCallback(function<void (HttpService* httpService)> cb);
        void setWorkerStopCallback(function<void (HttpService* httpService)> cb);
        void setConnectionCallback(ConnectionEventCallback cb);
        void setWriteCallback(ConnectionEventCallback cb);
        void setMessageCallback(function<void (TcpConnection* connection, HttpRequest* httpRequest, HttpResponse* httpResponse)> cb);
        void setCloseCallback(ConnectionEventCallback cb);
        void start();
    };

}
