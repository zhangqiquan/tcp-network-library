// +----------------------------------------------------------------------
// | tcp2 / Socket.h    [ 2022/11/28 16:37 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once

#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "InetAddress.h"
#include "Log.h"

namespace zhanshop {

    class Socket {
    protected:
        int _sockfd;
    public:
        Socket(int _sockfd); // 把创建fd丢到外面传过来
        void bind(const InetAddress &inetAddress); //调用bind绑定服务器地址信息
        void listen(int backlog = SOMAXCONN); // 监听套接字
        void connect(const InetAddress &inetAddress); // 连接上服务端
        void shutdownWrite(); // 调用shutdown关闭服务端写通道connect
        void setReuseAddr(); // 启用 SO_REUSEADDR地址复用
        void setReusePort(); // 启用 SO_REUSEPORT 允许其他线程也监听端口
        void setNonBlocking(); // 设置非阻塞
        int accept(sockaddr_in &clientAddr) const; // 调用accept接受新客户连接请求
        ssize_t read(void *buf, size_t count); // 从文件描述符读取
        ssize_t recv(void *buf, size_t len, int flags); // 从套接字接收消息
        ssize_t write(const void *buf, size_t count); // 向其他用户发送消息
        ssize_t send(const void *buf, size_t len, int flags); // 在套接字上发送消息
        void close(); // 关闭文件描述符
        const int sockfd() const;
    };

}
