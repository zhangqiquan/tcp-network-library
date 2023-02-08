// +----------------------------------------------------------------------
// | tcp2 / eventLoop.cc    [ 2022/11/28 16:29 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/EventLoop.h"

namespace zhanshop {

    EventLoop::EventLoop():
        _dispatcher(make_unique<Dispatcher>()),
        _wakeupFd(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)),
        _channel(make_unique<Channel>(this, _wakeupFd)),
        _timer(make_unique<Timer>())
    {
        this->_channel->setReadCallback(bind(&EventLoop::handleRead, this));
        this->_channel->setWriteCallback(bind(&EventLoop::handleWrite, this));
        this->_channel->setCloseCallback(bind(&EventLoop::handleClose, this));
        this->_channel->setDestroyCallback(bind(&EventLoop::handleRead, this));
        this->_channel->enableRead();
    }

    void EventLoop::setContext(void* context){
        this->_context = context;
    }

    void* EventLoop::getContext() const{
        return this->_context;
    }

    void EventLoop::runAfter(int msec, function<void()> callback){
        this->_timer->addTimer(this, msec, callback);
    }

    void EventLoop::runTick(int msec, function<void()> callback){
        this->_timer->addTimer(this, msec, callback);
    }

    void EventLoop::doPendingFunctors(){
        for (auto& functor : this->_wakeupFuns){
            functor(); // 就这里坏了
        }
        this->_wakeupFuns.clear();
    }

    void EventLoop::queueInLoop(fun cb){
        lock_guard<mutex> lock(this->_mutexLock);
        this->_wakeupFuns.push_back(move(cb));
        this->handleWrite();
    }
    // 批量任务 锁和通知在外层控制
    void EventLoop::queuesInLoop(fun cb){
        this->_wakeupFuns.push_back(move(cb));
    }

    void EventLoop::handleRead(){
        uint64_t value = 0;
        eventfd_read(this->_wakeupFd, &value);

        this->_mutexLock.lock();
        this->doPendingFunctors();
        this->_mutexLock.unlock();
    }

    void EventLoop::handleWrite(){
        ssize_t len = eventfd_write(this->_wakeupFd, 1);
        if(len == -1){
            LOG_ERROR("event通知失败，发送给："+ to_string(this->_wakeupFd));
        }
    }

    void EventLoop::handleClose(){

    }
    void EventLoop::handleError(){

    }

    void EventLoop::loop() {
        this->_tid = gettid();
        unsigned int length = 10240;
        epoll_event eventDatas[length];
        int wait = 0;
        while (!this->_quit){
            wait = this->_dispatcher->wait(eventDatas, length, -1);
            auto microtime = Date::microtime();
            for (int i = 0; i < wait; ++i) {
                auto channel = (Channel*)eventDatas[i].data.ptr;
                channel->setEvents(eventDatas[i].events);
                channel->HandlerEvent(microtime);
            }
        }
        // 如何包装子线程执行完毕主线程才退出(不管了 只管关闭掉所有连接即可)
    }

    void EventLoop::quit(){
        this->_quit = true;
    }

    void EventLoop::updateChannel(Channel* channel){
        if(channel->microtime() == -1){
            this->_dispatcher->add(channel->events(), channel->fd(), (void*) channel);
        }else{
            this->_dispatcher->edit(channel->events(), channel->fd(), (void*) channel);
        }
    }

    void EventLoop::removeChannel(Channel* channel){
        //LOG_DEBUG("EventLoop::removeChannel:"+ to_string(channel->fd()));
        this->_dispatcher->del(channel->fd()); // 销毁epoll 把这个通道挂载的保活也删除掉
    }

    mutex* EventLoop::mutexLock(){
        return &this->_mutexLock;
    }

    pid_t EventLoop::tid(){
        return this->_tid;
    }

}