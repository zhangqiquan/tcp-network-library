// +----------------------------------------------------------------------
// | zhanshop / IndexController.h    [ 2022/12/18 下午10:51 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include "../../../../include/http/HttpRequest.h"
#include "../../../../include/http/HttpResponse.h"
namespace zhanshop {

    class IndexController {
    public:
        string index(HttpRequest* httpRequest, HttpResponse* httpResponse){
            //httpResponse->test();
            httpResponse->setCode(200);
            httpResponse->setHeader("Connection", "keep-alive");
            httpResponse->setBody("123");
            httpResponse->send();
            return "";
            //return "123";
        }
    };

}
