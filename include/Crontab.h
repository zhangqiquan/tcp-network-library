// +----------------------------------------------------------------------
// | zhanshop / Crontab.hpp    [ 2022/12/6 下午9:22 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <sys/timerfd.h>
#include <memory>
#include "Callbacks.h"
#include "Channel.h"
#include "Socket.h"

using namespace std;

namespace zhanshop {

    class Crontab {
    protected:
        EventLoop* _eventLoop;
        unique_ptr<Socket> _socket;
        unique_ptr<Channel> _channel;
        ChannelEventCallback _callback;
    public:
        Crontab(EventLoop* eventLoop, int msec);
        void setCallback(ChannelEventCallback cb);

        void handleRead();
        void handleWrite();
        void handleClose();
        void handleError();
    };

}
