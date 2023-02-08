// +----------------------------------------------------------------------
// | tcp2 / Channel.cc    [ 2022/11/28 16:26 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/Channel.h"
#include "../include/EventLoop.h"

namespace zhanshop {

    Channel::Channel(EventLoop* eventLoop, int fd) : _eventLoop(eventLoop), _fd(fd) {

    }
    void Channel::setEvents(uint32_t events){ this->_events = events;}
    void Channel::setReadCallback(ChannelEventCallback cb){ this->_readCallback = std::move(cb); }
    void Channel::setWriteCallback(ChannelEventCallback cb){ this->_writeCallback = std::move(cb); }
    void Channel::setCloseCallback(ChannelEventCallback cb){ this->_closeCallback = std::move(cb); }
    void Channel::setErrorCallback(ChannelEventCallback cb){ this->_errorCallback = std::move(cb); }
    void Channel::setDestroyCallback(ChannelEventCallback cb) {this->_destroyCallback = std::move(cb);}

    double& Channel::microtime(){ return this->_microtime; }

    void Channel::HandlerEvent(double receiveTime)
    {
        this->_microtime = receiveTime;
        // 套接字意外关闭 并且不是一个读事件
        if((this->_events & EPOLLHUP) && !(this->_events & EPOLLIN)){
            //LOG_WARN("套接字意外关闭 并且不是一个读事件"+ to_string(this->_fd));
            this->_closeCallback(); // 执行关闭回调
            return;
        }
        // EPOLLNVAL文件描述符没有打开
        if(this->_events & EPOLLERR){
            //LOG_WARN("EPOLLNVAL文件描述符没有打开"+ to_string(this->_fd));
            this->_errorCallback();
            return;
        }
        // 读事件 数据（包括普通数据和优先数据）可读/高优先级数据可读，比如TCP带外数据/TCP链接被对方关闭，或者对方关闭写操作。它由GNU引入
        if (this->_events & (EPOLLIN)){
            this->_readCallback();
            return;
        }

        // 可写事件暂时用不着在读事件就返回给客户端了
        if(this->_events & EPOLLOUT){
            this->_writeCallback();
            return;
        }

    }
    int Channel::fd() const{
        return this->_fd;
    }

    uint32_t Channel::events() const{
        return this->_events;
    }

    void Channel::enableRead(){
        this->_events = EPOLLIN | EPOLLET;
        this->_eventLoop->updateChannel(this);
    } // 启用epoll读
    void Channel::enableWrite(){
        this->_events = EPOLLOUT | EPOLLET;
        this->_eventLoop->updateChannel(this);
    } // 启用epoll写
    void Channel::enableAll(){

    } // 启用epoll读写
    void Channel::disableAll(){
        this->_eventLoop->removeChannel(this);
    } // 删除epoll

    Channel::~Channel(){
        //LOG_DEBUG("Channel销毁了");
        //this->_destroyCallback();
    }
}