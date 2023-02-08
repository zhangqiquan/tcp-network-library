// +----------------------------------------------------------------------
// | tcp2 / eventLoop.h    [ 2022/11/28 16:29 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <sys/eventfd.h>
#include <memory>
#include <mutex>
#include <atomic>
#include "Callbacks.h"
#include "Dispatcher.h"
#include "Keepalive.h"
#include "Timer.h"
#include "Channel.h"

using namespace std;
using namespace zhanshop;

namespace zhanshop {

    class EventLoop {
    protected:
        unique_ptr<Dispatcher> _dispatcher;
        atomic<int>             _wakeupFd;
        unique_ptr<Channel> _channel;
        unique_ptr<Timer> _timer;
        atomic<bool> _quit = false;
        vector<fun> _wakeupFuns; // 挂载所有的监听方法
        void* _context = nullptr;
        mutex _mutexLock;
        pid_t _tid = 0;
    public:
        EventLoop();
        void setContext(void* context);
        void* getContext() const;
        mutex* mutexLock();
        void runAfter(int msec, function<void()> callback); // 添加一次性定时器
        void runTick(int msec, function<void()> callback); // 添加间隔定时器
        void runInLoop(fun cb);
        void queueInLoop(fun cb);
        void queuesInLoop(fun cb);// 如果是批量任务需要再调用这个方法之前加锁
        void doPendingFunctors();
        void handleRead();
        void handleWrite();
        void handleClose();
        void handleError();
        void loop();
        void quit();
        pid_t tid();
        void updateChannel(Channel* channel);
        void removeChannel(Channel* channel);

    };

}
