// +----------------------------------------------------------------------
// | tcp2 / Dispatcher.h    [ 2022/11/28 16:33 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Log.h"

namespace zhanshop {

    class Dispatcher {
    protected:
        int epollFd;
    public:
        Dispatcher();
        void add(int &&events, const int fd, void* channel);
        void add(int &&events, const int fd);
        void edit(int &&events, const int fd, void* channel);
        void edit(int &&events, const int fd);
        void del(const int fd);
        int wait(epoll_event* eventDatas, unsigned int length, int&& timeout);
    };

}
