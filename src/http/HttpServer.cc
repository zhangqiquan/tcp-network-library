// +----------------------------------------------------------------------
// | zhanshop / HttpServer.cc    [ 2022/12/17 下午4:41 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../../include/http/HttpServer.h"

namespace zhanshop {

    HttpServer::HttpServer(EventLoop* eventLoop, string ip, int port, unsigned int threadNum):
            _tcpServer(make_unique<TcpServer>(eventLoop, InetAddress(ip, port), threadNum, 5))
    {
        this->_tcpServer->setStartCallback(bind(&HttpServer::onStart, this, _1));
        this->_tcpServer->setStopCallback(bind(&HttpServer::onStop, this, _1));
        this->_tcpServer->seteventLoopStartCallback(bind(&HttpServer::onWorkerStart, this, _1));
        this->_tcpServer->seteventLoopStopCallback(bind(&HttpServer::onWorkerStop, this, _1));

        this->_tcpServer->setConnectionCallback(bind(&HttpServer::onConnection, this, _1));
        this->_tcpServer->setMessageCallback(bind(&HttpServer::onMessage, this, _1));
        this->_tcpServer->setCloseCallback(bind(&HttpServer::onClose, this, _1));
    }

    void HttpServer::onStart(TcpServer* _tcpServer){
        if(this->_startCallback != NULL) this->_startCallback(_tcpServer);
    }

    void HttpServer::onStop(TcpServer* _tcpServer){
        if(this->_stopCallback != NULL) this->_stopCallback(_tcpServer);
    }

    void HttpServer::onWorkerStart(EventLoop* eventLoop){
        if(this->_workerStartCallback != NULL){
            LOG_DEBUG("HttpServer::onWorkerStart");
            auto httpService = new HttpService;
            eventLoop->setContext((void*)httpService);
            this->_workerStartCallback(httpService);
        }else{
            LOG_DEBUG("HttpServer::onWorkerStart没有走HttpService");
        }
    }

    void HttpServer::onWorkerStop(EventLoop* eventLoop){
        if(this->_workerStopCallback != NULL){
            this->_workerStopCallback((HttpService*)eventLoop->getContext());
        }
    }

    void HttpServer::onConnection(TcpConnection* conn){
        this->_connectionCallback(conn);
        auto httpContext = new HttpContext;
        conn->setContext((void*)httpContext);
    }

    void HttpServer::onMessage(TcpConnection* conn){
        auto httpContext = (HttpContext*)conn->context();
        string buff = conn->buffer()->readData();
        // 现在的问题就在分包上面
        while (httpContext->parseRequest(buff)){
            auto httpResponse = new HttpResponse(conn->buffer());
            this->_messageCallback(conn, httpContext->httpRequest(), httpResponse);
        }

//        string msg = "";
//        msg += "HTTP/1.1 200 OK\r\n";
//        msg += "Connection: keep-alive\r\n";
//        msg += "Content-Length: 1\r\n";
//        msg += "\r\n";
//        msg += "1";
//        //sleep(100); // 这个会阻塞线程(除非重写它让它非阻塞)
//        //cout << "把请求来的数据给到客户端：" << client->receiverFd() << "》》" << client->result() << endl;
//        ::send(conn->socket()->sockfd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
    }

    void HttpServer::onClose(TcpConnection* conn){
        this->_closeCallback(conn);
    }

    void HttpServer::setStartCallback(function<void (TcpServer* tcpServer)> cb){
        this->_startCallback = move(cb);
    }

    void HttpServer::setStopCallback(function<void (TcpServer* tcpServer)> cb){
        this->_stopCallback = move(cb);
    }

    void HttpServer::setWorkerStartCallback(function<void (HttpService* httpService)> cb){
        this->_workerStartCallback = move(cb);
    }

    void HttpServer::setWorkerStopCallback(function<void (HttpService* httpService)> cb){
        this->_workerStopCallback = move(cb);
    }

    void HttpServer::setConnectionCallback(ConnectionEventCallback cb){
        this->_connectionCallback = move(cb);
    }

    void HttpServer::setWriteCallback(ConnectionEventCallback cb){
        this->_writeCallback = move(cb);
    }

    void HttpServer::setMessageCallback(function<void (TcpConnection* connection, HttpRequest* httpRequest, HttpResponse* httpResponse)> cb){
        this->_messageCallback = move(cb);
    }

    void HttpServer::setCloseCallback(ConnectionEventCallback cb){
        this->_closeCallback = move(cb);
    }

    void HttpServer::start(){
        this->_tcpServer->start();
    }


}