// +----------------------------------------------------------------------
// | zhanshop / HttpRequest.h    [ 2022/12/17 下午4:54 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <string>
#include <map>
using namespace std;

namespace zhanshop {

    class HttpRequest {
    protected:
        string _method = "";
        string _version = "";
        string _path = "";
        string _query = "";
        string _body = "";
        double _receiveTime;
        map<string, string> _headers;
        string _originalValue;
    public:
        HttpRequest();

        void setMethod(string method);
        void setPath(string path);
        void setQuery(string query);
        void setVersion(string version);
        void setHeader(string key, string val);
        void setBody(string body);
        string header(string key);
        map<string, string> headers();
        string& body();
        string& method();
        string& path();
        string& query();
        string& version();
    };

}
