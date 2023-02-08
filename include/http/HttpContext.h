// +----------------------------------------------------------------------
// | zhanshop / HttpContext.h    [ 2022/12/17 下午4:52 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include "HttpRequest.h"
using namespace std;

namespace zhanshop {

    class HttpContext {
    protected:
        unique_ptr<HttpRequest> _httpRequest;
        string _protocol = "HTTP/1.1";
    public:
        HttpContext();
        HttpRequest* httpRequest();
        void setProtocol(string protocol);
        string& protocol();
        bool parseRequest(string& buf);
        void parseHeader(string& buf);

        int websocketDecode(const char *buf, int len, char *decoded);
    };

}
