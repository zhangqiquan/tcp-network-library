// +----------------------------------------------------------------------
// | zhanshop / HttpRouter.h    [ 2022/12/18 下午10:14 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;

namespace zhanshop {

    class HttpRouter {
    protected:
        unordered_map<string, function<void(HttpRequest* httpRequest, HttpResponse* httpResponse)>> routers;

    public:
        void GET(string uri, function<void(HttpRequest* httpRequest, HttpResponse* httpResponse)> cb);
        void POST(string uri, function<void(HttpRequest* httpRequest, HttpResponse* httpResponse)> cb);
        void DELETE(string uri, function<void(HttpRequest* httpRequest, HttpResponse* httpResponse)> cb);
        void PUT(string uri, function<void(HttpRequest* httpRequest, HttpResponse* httpResponse)> cb);

        void notFound(HttpRequest* httpRequest, HttpResponse* httpResponse);
        void callback(HttpRequest* httpRequest, HttpResponse* httpResponse);
    };

}