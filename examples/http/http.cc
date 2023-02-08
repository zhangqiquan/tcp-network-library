// +----------------------------------------------------------------------
// | zhanshop-tcp2 / index.cc    [ 2022/12/1 上午10:16 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../../include/Error.h"
#include "../../include/EventLoop.h"
#include "../../include/http/HttpServer.h"
#include "Router.h"

int main(){
    EventLoop eventLoop;
    Error::listen();
    HttpServer server(&eventLoop, "0.0.0.0", 6202, 8);

    server.setWorkerStartCallback([](HttpService* httpService){
        Router router(httpService->httpRouter()); // 注册上路由
    });

    server.setConnectionCallback([](TcpConnection* conn){
        //LOG_INFO("连接*接受到了新的："+ to_string(conn->socket()->sockfd()));
    });

    // 这里拿到的是请求和响应function<void (TcpConnection* connection, HttpRequest* httpRequest, HttpResponse* httpResponse)>
    server.setMessageCallback([](TcpConnection* conn, HttpRequest* httpRequest, HttpResponse* httpResponse){
        // 验证路由是否存在
        string connection = httpRequest->header("Connection");
        httpResponse->longConnection(connection);
        auto httpService = (HttpService*)conn->eventLoop()->getContext();
        httpService->httpRouter()->callback(httpRequest, httpResponse);
//        // 去验证路由
//        httpResponse->setCode(200);
//        httpResponse->setVersion("HTTP/1.1");
//        httpResponse->setBody("ZHANGQIQUAN");
//        httpResponse->setcloseConnection(httpRequest->header("Connection") != "keep-alive" && httpRequest->header("Connection") != "Keep-Alive");
//        httpResponse->send();
    });

    server.setCloseCallback([](TcpConnection* conn){

    });

    server.start();

    return 0;
}