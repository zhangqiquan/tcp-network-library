// +----------------------------------------------------------------------
// | zhanshop / HttpService.h    [ 2022/12/18 下午10:25 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <memory>
#include "../TcpConnection.h"
#include "HttpRouter.h"

using namespace std;
using namespace zhanshop;

// 需要绑定到eventLoop上的服务类 路由的验证也在这上面
namespace zhanshop {

    class HttpService {
    protected:
        unique_ptr<HttpRouter> _httpRouter;
    public:
        HttpService();
        HttpRouter* httpRouter();

        void* route(TcpConnection* conn, HttpRequest* httpRequest, HttpResponse* httpResponse);

        int websocketDecode(unsigned char* buf, int len, char* decoded);
    };

}
