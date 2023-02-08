// +----------------------------------------------------------------------
// | zhanshop-tcp2 / Client.h    [ 2022/11/30 下午9:44 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <sys/uio.h>
#include <string>
#include "Socket.h"
#include "Callbacks.h"
#include "Dispatcher.h"
#include "Channel.h"

using namespace std;
using namespace zhanshop;

namespace zhanshop {

    class Client {
    protected:
        Dispatcher* _dispatcher;
        void* _response; // 响应类里面包含客户端fd
        string _ip;
        int _port;
        ClientEventCallback _sendCallback;
        ClientEventCallback _readCallback;

        int _fd = 0;
        Channel* _channel;
        string _result = "";
    public:
        Client(Dispatcher* dispatcher, void* response, string ip, int port, ClientEventCallback sendCallback, ClientEventCallback readCallback);
        void setChannel(Channel* channel);
        void connect();
        void handleRead();
        void handleWrite();
        void handleClose();
        void handleError();
        void handleDestroy();

        int fd();
        void* response();

        void send(string param);

        string result();
    };

}
