// +----------------------------------------------------------------------
// | tcp2 / Keepalive.h    [ 2022/11/28 16:30 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <memory>

using namespace std;

// 保活的话脱离于Tcp连接类 只是关联一个类
namespace zhanshop {
    class TcpConnection;
    class Keepalive {
    protected:
        weak_ptr<TcpConnection> _weakConn;
    public:
        Keepalive(weak_ptr<TcpConnection> weakConn);
        ~Keepalive();
    };

}
