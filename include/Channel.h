// +----------------------------------------------------------------------
// | tcp2 / Channel.h    [ 2022/11/28 16:26 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include "Callbacks.h"
#include "Dispatcher.h"

namespace zhanshop {
    class EventLoop;
    class Channel {
    protected:
        EventLoop* _eventLoop;
        const int  _fd;
        double _microtime = -1; // 如果它等于-1 就是这个epoll从来没有触发过
        uint32_t        _events;
        ChannelEventCallback _readCallback;
        ChannelEventCallback _writeCallback;
        ChannelEventCallback _closeCallback;
        ChannelEventCallback _errorCallback;
        ChannelEventCallback _destroyCallback;
        void update();
        void remove();
    public:
        Channel(EventLoop* eventLoop, int fd);
        double& microtime();
        void setEvents(uint32_t events);
        uint32_t events() const;
        void HandlerEvent(double receiveTime);
        void setReadCallback(ChannelEventCallback cb);
        void setWriteCallback(ChannelEventCallback cb);
        void setCloseCallback(ChannelEventCallback cb);
        void setErrorCallback(ChannelEventCallback cb);
        void setDestroyCallback(ChannelEventCallback cb);
        void enableRead(); // 启用epoll读
        void enableWrite(); // 启用epoll写
        void enableAll(); // 启用epoll读写
        void disableAll(); // 删除epoll
        int fd() const;
        ~Channel();
    };

}