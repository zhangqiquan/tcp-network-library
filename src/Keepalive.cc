// +----------------------------------------------------------------------
// | tcp2 / Keepalive.cc    [ 2022/11/28 16:30 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/Keepalive.h"
#include "../include/TcpConnection.h"
namespace zhanshop {

    Keepalive::Keepalive(weak_ptr<TcpConnection> weakConn){
        this->_weakConn = move(weakConn);
    }

    Keepalive::~Keepalive(){
        if(this->_weakConn.lock() && this->_weakConn.expired() == false){
            LOG_WARN("保活超时关闭"+ to_string(this->_weakConn.lock()->socket()->sockfd()));
            ::shutdown(this->_weakConn.lock()->socket()->sockfd(), SHUT_WR);
        }
    }
}