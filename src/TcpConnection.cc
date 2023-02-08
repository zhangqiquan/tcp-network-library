// +----------------------------------------------------------------------
// | tcp2 / TcpConnection.cc    [ 2022/11/28 16:31 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/TcpConnection.h"
#include "../include/EventLoop.h"

namespace zhanshop {
    TcpConnection::TcpConnection(EventLoop* eventLoop, InetAddress clientAddr):
        _eventLoop(eventLoop),
        _socket(make_unique<Socket>(clientAddr.fd())),
        _clientAddr(clientAddr),
        _buffer(make_unique<Buffer>(clientAddr.fd())),
        _channel(make_unique<Channel>(eventLoop, clientAddr.fd()))
    {
        this->_channel->setReadCallback(bind(&TcpConnection::handleRead, this));
        this->_channel->setWriteCallback(bind(&TcpConnection::handleWrite, this));
        this->_channel->setCloseCallback(bind(&TcpConnection::handleClose, this));
        this->_channel->setErrorCallback(bind(&TcpConnection::handleError, this));
    }

    void TcpConnection::setConnectionCallback(ConnectionEventCallback cb){
        this->_connectionCallback = cb;
    }
    void TcpConnection::setMessageCallback(ConnectionEventCallback cb){
        this->_messageCallback = cb;
    }
    void TcpConnection::setWriteCallback(ConnectionEventCallback cb){
        this->_writeCallback = cb;
    }
    void TcpConnection::setCloseCallback(ConnectionEventCallback cb){
        this->_closeCallback = cb;
    }
    // 处理接收tcp连接
    void TcpConnection::connectEstablished(){
        // 做保活处理
        // shared_ptr<TcpConnection>(this); // 会报错terminate called after throwing an instance of 'std::bad_function_call' what():  bad_function_call
        this->_channel->enableRead(); // 这个玩意在主线程
        this->_connectionCallback(this);
    }
    // 处理发起连接的
    void TcpConnection::clientEstablished(){
        // 做保活处理
        this->_channel->enableWrite(); // 这个玩意在主线程
        this->socket()->connect(this->_clientAddr); // 去触发连接
        this->_connectionCallback(this);
    }

    void TcpConnection::setKeepalive(weak_ptr<Keepalive> keepalive){
        this->_keepalive = keepalive;
    }

    weak_ptr<Keepalive>& TcpConnection::keepalive(){
        return this->_keepalive;
    }

    void TcpConnection::setContext(void* context){
        this->_context = move(context);
    }

    void* TcpConnection::context(){
        return this->_context;
    }

    void TcpConnection::handleRead(){
        if(this->_buffer->read() == 0){
            this->handleClose();
            return;
        }
        this->_messageCallback(this);
    }
    void TcpConnection::handleWrite(){
        this->_writeCallback(this);
        // 改成仅读
        this->_channel->enableRead();
    }
    void TcpConnection::handleClose(){
        this->_channel->disableAll(); // 销毁epoll
        this->_closeCallback(this); // 调用主线程的关闭销毁资源
    }
    void TcpConnection::handleError(){
        this->_closeCallback(this);
    }
    void TcpConnection::handleDestroy(){

    }

    EventLoop* TcpConnection::eventLoop(){
        return this->_eventLoop;
    }

    Socket* TcpConnection::socket() const{
        return this->_socket.get();
    }

    Buffer* TcpConnection::buffer(){
        return this->_buffer.get();
    }

    InetAddress* TcpConnection::clientAddr(){
        return &this->_clientAddr;
    }

    TcpConnection::~TcpConnection(){
        //LOG_WARN("TcpConnection销毁了"+ to_string(this->socket()->sockfd()));
    }
}