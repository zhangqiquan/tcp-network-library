// +----------------------------------------------------------------------
// | tcp2 / Callbacks.h    [ 2022/11/28 16:34 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <functional>

using namespace std;
using namespace placeholders;

namespace zhanshop {
    class TcpServer;
    class EventLoop;
    class TcpConnection;
    class Client;
    using TcpServerEventCallback = function<void (TcpServer* tcpServer)>;
    using eventLoopEventCallback = function<void (EventLoop* eventLoop)>; // 子线程事件(启动 停止)
    using ChannelEventCallback = function<void()>;
    using ConnectionEventCallback = function<void (TcpConnection* conn)>; // 子线程连接事件回调(连接 消息 断开)
    using ClientEventCallback = function<void (Client* client)>;
    using fun = function<void()>;
}
