// +----------------------------------------------------------------------
// | tcp2 / Timer.cc    [ 2022/11/28 16:25 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/Timer.h"
#include "../include/EventLoop.h"

using namespace zhanshop;

namespace zhanshop {
    Timer::Timer(){

    }

    void Timer::addTimer(EventLoop* eventLoop, int msec, function<void()> callback){
        auto crontab = new Crontab(eventLoop, msec);
        crontab->setCallback(callback);
    }
}