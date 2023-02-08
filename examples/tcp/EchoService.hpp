// +----------------------------------------------------------------------
// | zhanshop / echo.hpp    [ 2022/12/1 上午10:21 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <memory>
#include "../../include/TcpClient.h"

using namespace std;
using namespace zhanshop;

namespace zhanshop {

    class EchoService {
    protected:
        Dispatcher* _dispatcher;
        unique_ptr<TcpClient> _tcpClient;
    public:
        EchoService(Dispatcher* dispatcher):_dispatcher(dispatcher){

        }

        TcpClient* tcpClient(){
            return this->_tcpClient.get();
        }

    };

}
