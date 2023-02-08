// +----------------------------------------------------------------------
// | zhanshop / echo.hpp    [ 2022/12/1 上午10:21 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include "../../include/InetAddress.h"
#include "../../include/TcpServer.h"
#include "../../include/TcpConnection.h"
#include "../../include/EventLoop.h"
#include "EchoService.hpp"

using namespace placeholders;
using namespace zhanshop;

namespace zhanshop {

    class EchoServer {
    protected:
        unique_ptr<TcpServer> _tcpServer;

        void onStart(TcpServer* _tcpServer){

        }

        void onStop(TcpServer* _tcpServer){

        }

        void onWorkerStart(EventLoop* eventLoop){
            // 做请求
        }

        void onWorkerStop(EventLoop* eventLoop){
        }

        void onConnection(TcpConnection* conn){
            LOG_WARN("onConnection:"+ to_string(conn->socket()->sockfd()));
        }

        void onMessage(TcpConnection* conn){
            string msg = "";
            msg += "HTTP/1.1 200 OK\r\n";
            //msg += "Connection: keep-alive\r\n";
            msg += "Content-Length: 1\r\n";
            msg += "\r\n";
            msg += "1";
            conn->buffer()->send(msg);
            ::shutdown(conn->socket()->sockfd(), SHUT_WR);
        }

        void onClose(TcpConnection* conn){
        }

    public:
        EchoServer(EventLoop* eventLoop, string ip, int port, unsigned int threadNum):
            _tcpServer(make_unique<TcpServer>(eventLoop, InetAddress(ip, port), threadNum, 5))
        {
            this->_tcpServer->setStartCallback(bind(&EchoServer::onStart, this, _1));
            this->_tcpServer->setStopCallback(bind(&EchoServer::onStop, this, _1));
            this->_tcpServer->seteventLoopStartCallback(bind(&EchoServer::onWorkerStart, this, _1));
            this->_tcpServer->seteventLoopStopCallback(bind(&EchoServer::onWorkerStop, this, _1));

            this->_tcpServer->setConnectionCallback(bind(&EchoServer::onConnection, this, _1));
            this->_tcpServer->setMessageCallback(bind(&EchoServer::onMessage, this, _1));
            this->_tcpServer->setCloseCallback(bind(&EchoServer::onClose, this, _1));
        }

        void start(){
            this->_tcpServer->start();
        }
    };

}
