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

            string response = "HTTP/1.1 101 Web Socket Protocol Handshake\r\n";
            response += "Upgrade: websocket\r\n";
            response += "Connection: Upgrade\r\n";
//            response += "WebSocket-Origin: 127.0.0.1\r\n";
//            response += "WebSocket-Location: ws://127.0.0.1:6201/websocket/websocket\r\n";
            response += "Sec-WebSocket-Accept:"+httpResponse->acceptKey(httpRequest->header("Sec-WebSocket-Key"))+"\r\n\r\n";
            httpResponse->sendText(response);
            return "";
            //return "123";
        }
    };

}
