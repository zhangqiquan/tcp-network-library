// +----------------------------------------------------------------------
// | zhanshop / HttpResponse.h    [ 2022/12/17 下午4:55 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <string>
#include <map>
#include "../Buffer.h"
#include "../helper/Encrypt.h"
#include "../helper/Base64.h"

using namespace std;
using namespace zhanshop;

namespace zhanshop {
    class HttpResponse {
    protected:
        Buffer* _buffer;
        unsigned int _code;
        string _message = "OK";
        string _version = "HTTP/1.1";
        bool _closeConnection = false;
        std::map<string, string> _headers;
        string _body = "";
    public:
        HttpResponse(Buffer* buffer);
        void setCode(unsigned int code);
        void setMessage(string message);
        void setVersion(string version);
        void longConnection(string& connection);
        void setHeader(string key, string val);
        void setBody(string body);
        void send();
        void sendText(string text);
        string acceptKey(string key);
        void test();
    };

}