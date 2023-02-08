// +----------------------------------------------------------------------
// | zhanshop-tcp2 / index.cc    [ 2022/12/1 上午10:16 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../../include/Error.h"
#include "../../include/EventLoop.h"
#include "EchoServer.hpp"

int main(){
    EventLoop eventLoop;
    Error::listen();
    EchoServer server(&eventLoop, "0.0.0.0", 6201, 8);
    server.start();

    return 0;
}