// +----------------------------------------------------------------------
// | zhanshop / router.h    [ 2022/12/18 下午10:21 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include "../../include/http/HttpRouter.h"
#include "api/controller/IndexController.h"
using namespace std;
using namespace zhanshop;

// 路由属于httpServer的一部分
class Router {
public:
    Router(HttpRouter* httpRouter){
        auto indexController = new IndexController;
        httpRouter->GET("/v3.0.0/flow", bind(&IndexController::index, indexController, _1, _2));
    }
};
