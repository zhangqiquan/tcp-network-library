// +----------------------------------------------------------------------
// | tcp2 / Timer.h    [ 2022/11/28 16:25 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include "Crontab.h"

namespace zhanshop {
    class EventLoop;
    class Timer {
    protected:
        //EventLoop* _eventLoop;
    public:
        Timer();
        void addTimer(EventLoop* eventLoop, int msec, function<void()> callback);
    };

}
