// +----------------------------------------------------------------------
// | zhanshop / websocket.cc    [ 2022/12/23 上午11:16 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------

// 让其路由能支持http请求参数同时支持websocket的message请求
#include "../../include/Error.h"
#include "../../include/EventLoop.h"
#include "../../include/http/WebSocketServer.h"
#include "Router.h"

int main(){
    EventLoop eventLoop;
    Error::listen();
    WebSocketServer server(&eventLoop, "0.0.0.0", 6201, 1);

    server.setWorkerStartCallback([](HttpService* httpService){
        Router router(httpService->httpRouter()); // 注册上路由
    });

    server.setConnectionCallback([](TcpConnection* conn){
        LOG_INFO("连接*接受到了新的："+ to_string(conn->socket()->sockfd()));
    });

    // 这里拿到的是请求和响应function<void (TcpConnection* connection, HttpRequest* httpRequest, HttpResponse* httpResponse)>
    server.setMessageCallback([](TcpConnection* conn, HttpRequest* httpRequest, HttpResponse* httpResponse){
        // 验证路由是否存在
        string connection = httpRequest->header("Connection");
        httpResponse->longConnection(connection);
        auto httpService = (HttpService*)conn->eventLoop()->getContext();
        if(httpRequest->header("Upgrade") == "websocket")
        // 思路还是这样首次如果是websocket 那么后面都都是
        httpService->httpRouter()->callback(httpRequest, httpResponse);
//        // 去验证路由
//        httpResponse->setCode(200);
//        httpResponse->setVersion("HTTP/1.1");
//        httpResponse->setBody("ZHANGQIQUAN");
//        httpResponse->setcloseConnection(httpRequest->header("Connection") != "keep-alive" && httpRequest->header("Connection") != "Keep-Alive");
//        httpResponse->send();
    });

    server.setMessageCallback([](TcpConnection* conn, string data){
        auto httpContext = (HttpContext*)conn->context();
        LOG_DEBUG(data);
    });

    server.setCloseCallback([](TcpConnection* conn){

    });

    server.start();

    return 0;
}