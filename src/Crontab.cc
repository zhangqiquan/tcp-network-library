// +----------------------------------------------------------------------
// | zhanshop / Crontab.cc    [ 2022/12/6 下午9:22 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/Crontab.h"

namespace zhanshop {
    // 仅支持到毫秒
    Crontab::Crontab(EventLoop* eventLoop, int msec):_eventLoop(eventLoop),_socket(make_unique<Socket>(timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK))),_channel(make_unique<Channel>(eventLoop, _socket->sockfd())){

        // 0.1 秒=100000000 纳秒
        //   1 秒=1000000000 纳秒
        struct itimerspec new_value;
        new_value.it_value.tv_sec  = msec / 1000;//第一次定时时间 // 第一次执行5秒
        new_value.it_value.tv_nsec = (msec % 1000) * 1000000;

        new_value.it_interval.tv_sec = msec / 1000;//后面每次间隔的时间 如果只执行首次那后面应该都是0
        new_value.it_interval.tv_nsec = (msec % 1000) * 1000000;

        int ret = timerfd_settime(this->_socket->sockfd(), 0, &new_value, NULL);//设置这个描述符的定时使时间
        if(ret == -1){
            perror("timerfd_settime:");
            LOG_FATAL("创建保活定时器fd失败");
        }

        this->_channel->setReadCallback(bind(&Crontab::handleRead, this));
        this->_channel->setWriteCallback(bind(&Crontab::handleWrite, this));
        this->_channel->setCloseCallback(bind(&Crontab::handleClose, this));
        this->_channel->setErrorCallback(bind(&Crontab::handleError, this));
    }

    void Crontab::setCallback(ChannelEventCallback cb){
        this->_callback = move(cb);
        this->_channel->enableRead(); // 回调方法设置好了就挂载到epoll 等事件到来就回调事件
    }

    void Crontab::handleRead(){
        uint64_t exp;
        read(this->_socket->sockfd(), &exp, sizeof(uint64_t));
        this->_callback(); // 执行回调方法
    }
    void Crontab::handleWrite(){
        LOG_DEBUG("连接可以写了");
    }
    void Crontab::handleClose(){
        LOG_DEBUG("连接关闭了");
    }
    void Crontab::handleError(){
        //LOG_DEBUG("连接发生错误了");
    }
}