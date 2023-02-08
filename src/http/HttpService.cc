// +----------------------------------------------------------------------
// | zhanshop / HttpService.cc    [ 2022/12/18 下午10:25 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../../include/http/HttpService.h"

namespace zhanshop {

    HttpService::HttpService():_httpRouter(make_unique<HttpRouter>()){

    }

    HttpRouter* HttpService::httpRouter(){
        return this->_httpRouter.get();
    }

}
