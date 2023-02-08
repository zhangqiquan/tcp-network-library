// +----------------------------------------------------------------------
// | tcp2 / Buffer.h    [ 2022/11/28 16:32 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <sys/uio.h>
#include <string>
#include "Log.h"
using namespace std;

namespace zhanshop {

    class Buffer {
    protected:
        int _fd;
        int _readLen;
        char _readData[5120];
    public:
        Buffer(int fd);
        int read();
        int send(string msg);
        char * readData();
        int& readLen();
        int& fd();
    };

}
