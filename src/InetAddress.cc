// +----------------------------------------------------------------------
// | tcp2 / InetAddress.cc    [ 2022/11/28 16:36 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/InetAddress.h"

namespace zhanshop {

    InetAddress::InetAddress(std::string ip, int port) {
        struct sockaddr_in serverAddr;
        serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        this->_addr = serverAddr;
    }

    InetAddress::InetAddress(int fd, const sockaddr_in &addr):_fd(fd), _addr(addr) {
    }

    void InetAddress::setFd(int fd){
        this->_fd = move(fd);
    }

    string InetAddress::ip() const {
        char clientIp[255];
        memcpy(clientIp, inet_ntop(AF_INET, &this->_addr.sin_addr.s_addr, clientIp, sizeof(clientIp)), 255);
        return clientIp;
    }

    int InetAddress::port() const {
        return ntohs(this->_addr.sin_port);
    }

    int InetAddress::fd() const{
        return this->_fd;
    }

    const sockaddr_in InetAddress::addr() const {
        return this->_addr;
    }
}