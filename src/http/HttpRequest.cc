// +----------------------------------------------------------------------
// | zhanshop / HttpRequest.cc    [ 2022/12/17 下午4:54 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../../include/http/HttpRequest.h"
#include <iostream>
using namespace std;
namespace zhanshop {
    HttpRequest::HttpRequest(){

    }

    void HttpRequest::setMethod(string method){
        this->_method = move(method); // 是不是就是这个move 导致的问题 这些地方不能使用move 因为那原始内存数据最终会被销毁的 X
    }

    void HttpRequest::setPath(string path){
        this->_path = move(path);
    }

    void HttpRequest::setQuery(string query){
        if(query == "/") query = "";
        this->_query = move(query);
    }

    void HttpRequest::setVersion(string version){
        this->_version = move(version);
    }

    void HttpRequest::setHeader(string key, string val){
        this->_headers[key] = val;
    }

    void HttpRequest::setBody(string body){
        this->_body = body;
    }

    string HttpRequest::header(string key){
        auto search = this->_headers.find(key);
        if (search != this->_headers.end()) {
            return search->second;
        }
        return "";
    }

    map<string, string> HttpRequest::headers(){
        return this->_headers;
    }

    string& HttpRequest::body(){
        return this->_body;
    }
    string& HttpRequest::method(){
        return this->_method;
    }
    string& HttpRequest::path(){
        return this->_path;
    }
    string& HttpRequest::query(){
        return this->_query;
    }
    string& HttpRequest::version(){
        return this->_version;
    }
}