// +----------------------------------------------------------------------
// | tcp2 / Dispatcher.cc    [ 2022/11/28 16:33 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/Dispatcher.h"

namespace zhanshop {

    Dispatcher::Dispatcher():epollFd(epoll_create(1)) {
    }

    void Dispatcher::add(int &&events, const int fd) {
        epoll_event eventData;
        eventData.events = events;
        eventData.data.fd = fd;
        epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &eventData);
    }

    void Dispatcher::add(int &&events, const int fd, void* channel) {
        epoll_event eventData;
        eventData.events = events;
        eventData.data.ptr = channel;
        epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &eventData);
    }

    void Dispatcher::edit(int &&events, const int fd, void* channel) {
        epoll_event eventData;
        eventData.events = events;
        eventData.data.ptr = channel;
        epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &eventData);
    }

    void Dispatcher::edit(int &&events, const int fd) {
        epoll_event eventData;
        eventData.events = events;
        eventData.data.fd = fd;
        epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &eventData);
    }

    int Dispatcher::wait(epoll_event *eventDatas, unsigned int length, int &&timeout) {
        return epoll_wait(this->epollFd, eventDatas, length, timeout);
    }

    void Dispatcher::del(const int fd) {
        epoll_ctl(this->epollFd, EPOLL_CTL_DEL, fd, nullptr);
    }

}