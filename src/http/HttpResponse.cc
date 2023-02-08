// +----------------------------------------------------------------------
// | zhanshop / HttpResponse.cc    [ 2022/12/17 下午4:55 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../../include/http/HttpResponse.h"
#include "../../include/Socket.h"
namespace zhanshop {

    HttpResponse::HttpResponse(Buffer* buffer){
        this->_buffer = buffer;
    }
    void HttpResponse::setCode(unsigned int code){
        this->_code = move(code);
    }
    void HttpResponse::setMessage(string message){
        this->_message = move(message);
    }

    void HttpResponse::setVersion(string version){
        this->_version = move(version);
    }

    void HttpResponse::longConnection(string& connection){
        this->_headers["Connection"] = connection;
        if(connection != "keep-alive" && connection != "Keep-Alive"){
            //LOG_WARN(connection);
            this->_closeConnection = true;
        } else{
            //LOG_WARN(connection);
            this->_closeConnection = false;
        }
    }


    void HttpResponse::setHeader(string key, string val){
        this->_headers[move(key)] = move(val);
    }

    void HttpResponse::setBody(string body){
        this->setHeader("Content-Length", to_string(body.length()));
        //this->setHeader("Connection", "keep-alive"); // 不加这个东西会出现报错(这是一个问题明天需要好好处理以下)
        this->_body = move(body);
    }
    // 问题设想 如果没有做长连接的处理，客户端采用长连接，服务器端不采用，客户端会销毁这个连接
    // 先解决第一个问题 那就是客户端没有 Connection = keep-alive的情况
    void HttpResponse::send(){
        string buff = "";
        buff += this->_version+" "+ to_string(this->_code)+" "+this->_message+"\r\n";

        for (const auto& header : this->_headers){
            buff += header.first + ": "+header.second+"\r\n";
        }
        // 只支持长连接 还是出现了崩溃
        buff += "\r\n";
        buff += this->_body;
        this->_buffer->send(buff);
        // 这里有问题呀
        if(this->_closeConnection){
            ::shutdown(this->_buffer->fd(), SHUT_WR);
            //LOG_WARN("_closeConnection："+ to_string(this->_buffer->fd()));
        }
    }

    string HttpResponse::acceptKey(string key){
        string make = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        //const unsigned char *sha1 = zhanshop::Encrypt::sha1(make); // 拿到加密值
        string sha1 = Encrypt::sha1(make, true);
        // 按网上说的这里应该少了一步http://www.manongjc.com/detail/50-pywtolyjpkfspeh.html
        // 必须要有那一步 不然我这个比别人的长

        //XLMp8pL0ywWYIxZGNVHycQVlDAM= swoole的
        //RTcxNDZFMzE2RDgwNDMwRkRFNkI= 自己的

        //string tolower = zhanshop::Str::tolower(sha1);
        //string subs = zhanshop::Str::substr(tolower, 0 , 20);
        return Base64::encode(sha1);
    }

    void HttpResponse::sendText(string text){
        ::send(this->_buffer->fd(), text.c_str(), text.length(), MSG_NOSIGNAL);
    }

    void HttpResponse::test(){
        string msg = "";
        msg += "HTTP/1.1 200 OK\r\n";
        msg += "Connection: keep-alive\r\n";
        msg += "Content-Length: 1\r\n";
        msg += "\r\n";
        msg += "1";
        //sleep(100); // 这个会阻塞线程(除非重写它让它非阻塞)
        //cout << "把请求来的数据给到客户端：" << client->receiverFd() << "》》" << client->result() << endl;
        ::send(this->_buffer->fd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
    }
}