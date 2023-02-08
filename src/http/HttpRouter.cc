// +----------------------------------------------------------------------
// | zhanshop / HttpRouter.cc    [ 2022/12/18 下午10:14 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../../include/http/HttpRouter.h"

namespace zhanshop {
    void HttpRouter::GET(string uri, function<void(HttpRequest* httpRequest, HttpResponse* httpResponse)> cb){
        this->routers[uri+":GET"] = cb;
    }

    void HttpRouter::POST(string uri, function<void(HttpRequest* httpRequest, HttpResponse* httpResponse)> cb){
        this->routers[uri+":POST"] = cb;
    }

    void HttpRouter::DELETE(string uri, function<void(HttpRequest* httpRequest, HttpResponse* httpResponse)> cb){
        this->routers[uri+":DELETE"] = cb;
    }

    void HttpRouter::PUT(string uri, function<void(HttpRequest* httpRequest, HttpResponse* httpResponse)> cb){
        this->routers[uri+":PUT"] = cb;
    }
    // 404的错误
    void HttpRouter::notFound(HttpRequest* httpRequest, HttpResponse* httpResponse){
        httpResponse->setCode(404);
        httpResponse->setVersion("HTTP/1.1");
        httpResponse->setBody("404访问不存在");
        string connection = httpRequest->header("Connection");
        httpResponse->longConnection(connection);
        httpResponse->send();
    }

    void HttpRouter::callback(HttpRequest* httpRequest, HttpResponse* httpResponse){
        auto search = this->routers.find(httpRequest->path()+":"+httpRequest->method());
        if (search != this->routers.end()) {
            search->second(httpRequest, httpResponse);
        } else {
            this->notFound(httpRequest, httpResponse);
        }
    }
}