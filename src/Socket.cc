// +----------------------------------------------------------------------
// | tcp2 / Socket.cc    [ 2022/11/28 16:37 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/Socket.h"

namespace zhanshop {

    Socket::Socket(int _sockfd):_sockfd(_sockfd) {
    }

    void Socket::bind(const zhanshop::InetAddress &inetAddress) {
        sockaddr_in addr = inetAddress.addr();
        int ret = ::bind(this->_sockfd, (struct sockaddr*)&addr, sizeof(addr));
        if(ret == -1){
            perror("bind: ");
            LOG_FATAL("socket绑定失败");
        }
    }

    void Socket::listen(int backlog){
        if(::listen(this->_sockfd, backlog) == -1){
            perror("listen:");
            LOG_FATAL("socket监听失败");
        }
    }

    void Socket::connect(const zhanshop::InetAddress &inetAddress) {
        sockaddr_in addr = inetAddress.addr();
        ::connect(this->_sockfd, (struct sockaddr*) &addr, sizeof(addr));
    }

    void Socket::shutdownWrite() {
        if (::shutdown(this->_sockfd, SHUT_WR) < 0){
            perror("shutdown:");
            LOG_ERROR(to_string(this->_sockfd)+"socket关闭写入失败");
        }
    }

    void Socket::setReuseAddr() {
        int optval = 1;
        ::setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    }

    void Socket::setReusePort() {
        int optval = 1;
        ::setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    }

    void Socket::setNonBlocking() {
        int flag = fcntl(this->_sockfd, F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(this->_sockfd, F_SETFL, flag);
    }

    int Socket::accept(sockaddr_in &clientAddr) const {
        socklen_t addrLen = (socklen_t)sizeof(clientAddr);
        const int clientFd = ::accept4(this->_sockfd, (struct sockaddr *)&clientAddr, &addrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);
        return clientFd;
    }

    ssize_t Socket::read(void *buf, size_t count) {
        return ::read(this->_sockfd, buf, count);
    }

    ssize_t Socket::recv(void *buf, size_t len, int flags) {
        return ::recv(this->_sockfd, buf, len, flags);
    }

    ssize_t Socket::write(const void *buf, size_t count) {
        return this->send(buf, count, MSG_NOSIGNAL); // 屏蔽给客户端已经断线的客户端发消息的错误信号13
    }

    ssize_t Socket::send(const void *buf, size_t len, int flags) {
        return ::send(this->_sockfd, buf, len, flags);
    }

    void Socket::close() {
        if (::close(this->_sockfd) == -1){
            perror("close:");
            LOG_ERROR(to_string(this->_sockfd)+"socket关闭失败");
        }
    }

    const int Socket::sockfd() const{
        return this->_sockfd;
    }
}