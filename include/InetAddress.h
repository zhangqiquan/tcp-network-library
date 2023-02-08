// +----------------------------------------------------------------------
// | tcp2 / InetAddress.h    [ 2022/11/28 16:36 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <cstring>

using namespace std;

namespace zhanshop {

    class InetAddress {
    protected:
        int _fd = -1;
        sockaddr_in _addr;
    public:
        InetAddress(string ip, int port);
        InetAddress(int fd, const sockaddr_in &addr);
        void setFd(int fd);
        string ip() const;
        int port() const;
        int fd() const;

        const sockaddr_in addr() const;

    };

}
