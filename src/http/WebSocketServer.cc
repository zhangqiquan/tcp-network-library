// +----------------------------------------------------------------------
// | zhanshop / WebSocketServer.cc    [ 2022/12/24 上午7:27 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../../include/http/WebSocketServer.h"

namespace zhanshop {

    WebSocketServer::WebSocketServer(EventLoop* eventLoop, string ip, int port, unsigned int threadNum):
            _tcpServer(make_unique<TcpServer>(eventLoop, InetAddress(ip, port), threadNum, 60))
    {
        this->_tcpServer->setStartCallback(bind(&WebSocketServer::onStart, this, _1));
        this->_tcpServer->setStopCallback(bind(&WebSocketServer::onStop, this, _1));
        this->_tcpServer->seteventLoopStartCallback(bind(&WebSocketServer::onWorkerStart, this, _1));
        this->_tcpServer->seteventLoopStopCallback(bind(&WebSocketServer::onWorkerStop, this, _1));

        this->_tcpServer->setConnectionCallback(bind(&WebSocketServer::onConnection, this, _1));
        this->_tcpServer->setMessageCallback(bind(&WebSocketServer::onMessage, this, _1));
        this->_tcpServer->setCloseCallback(bind(&WebSocketServer::onClose, this, _1));
    }

    void WebSocketServer::onStart(TcpServer* _tcpServer){
        if(this->_startCallback != NULL) this->_startCallback(_tcpServer);
    }

    void WebSocketServer::onStop(TcpServer* _tcpServer){
        if(this->_stopCallback != NULL) this->_stopCallback(_tcpServer);
    }

    void WebSocketServer::onWorkerStart(EventLoop* eventLoop){
        if(this->_workerStartCallback != NULL){
            LOG_DEBUG("WebSocketServer::onWorkerStart");
            auto httpService = new HttpService;
            eventLoop->setContext((void*)httpService);
            this->_workerStartCallback(httpService);
        }else{
            LOG_DEBUG("WebSocketServer::onWorkerStart没有走HttpService");
        }
    }

    void WebSocketServer::onWorkerStop(EventLoop* eventLoop){
        if(this->_workerStopCallback != NULL){
            this->_workerStopCallback((HttpService*)eventLoop->getContext());
        }
    }

    void WebSocketServer::onConnection(TcpConnection* conn){
        this->_connectionCallback(conn);
        auto httpContext = new HttpContext;
        conn->setContext((void*)httpContext);
    }

    void WebSocketServer::onMessage(TcpConnection* conn){
        auto httpContext = (HttpContext*)conn->context();
        if(httpContext->protocol() == "websocket"){
            char demsg[5120];
            memset(demsg, 0, sizeof(demsg));
            httpContext->websocketDecode(conn->buffer()->readData(), conn->buffer()->readLen(), demsg);
            this->_webSocketMessageCallback(conn, demsg);
            return;
        }
        string buff = conn->buffer()->readData();
        // 现在的问题就在分包上面
        while (httpContext->parseRequest(buff)){
            httpContext->setProtocol(httpContext->httpRequest()->header("Upgrade"));
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

    void WebSocketServer::onClose(TcpConnection* conn){
        this->_closeCallback(conn);
    }

    void WebSocketServer::setStartCallback(function<void (TcpServer* tcpServer)> cb){
        this->_startCallback = move(cb);
    }

    void WebSocketServer::setStopCallback(function<void (TcpServer* tcpServer)> cb){
        this->_stopCallback = move(cb);
    }

    void WebSocketServer::setWorkerStartCallback(function<void (HttpService* httpService)> cb){
        this->_workerStartCallback = move(cb);
    }

    void WebSocketServer::setWorkerStopCallback(function<void (HttpService* httpService)> cb){
        this->_workerStopCallback = move(cb);
    }

    void WebSocketServer::setConnectionCallback(ConnectionEventCallback cb){
        this->_connectionCallback = move(cb);
    }

    void WebSocketServer::setWriteCallback(ConnectionEventCallback cb){
        this->_writeCallback = move(cb);
    }

    void WebSocketServer::setMessageCallback(function<void (TcpConnection* connection, HttpRequest* httpRequest, HttpResponse* httpResponse)> cb){
        this->_messageCallback = move(cb);
    }

    void WebSocketServer::setMessageCallback(function<void (TcpConnection* connection, string data)> cb){
        this->_webSocketMessageCallback = move(cb);
    }

    void WebSocketServer::setCloseCallback(ConnectionEventCallback cb){
        this->_closeCallback = move(cb);
    }

    void WebSocketServer::start(){
        this->_tcpServer->start();
    }


}