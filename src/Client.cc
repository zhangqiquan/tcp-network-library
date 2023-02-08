// +----------------------------------------------------------------------
// | zhanshop-tcp2 / Client.cc    [ 2022/11/30 下午9:44 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/Client.h"

namespace zhanshop {
    Client::Client(Dispatcher *dispatcher, void *response, string ip, int port, ClientEventCallback sendCallback, ClientEventCallback readCallback):
    _dispatcher(dispatcher), _response(response), _ip(ip), _port(port), _sendCallback(sendCallback), _readCallback(readCallback)
    {
        this->_fd = socket(AF_INET, SOCK_STREAM, 0);
        cout << "创建tcp客户端连接fd：" << this->_fd << endl;
        if(this->_fd == -1)
        {
            perror("socket");
            exit(0);
        }

        int flag = fcntl(this->_fd, F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(this->_fd, F_SETFL, flag);
        LOG_DEBUG("Client::Client:"+ to_string(this->_fd));
    }

    void Client::setChannel(Channel *channel) {
        this->_channel = channel;
    }

    void Client::connect(){
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(this->_port);   // 大端端口
        inet_pton(AF_INET, this->_ip.c_str(), &addr.sin_addr.s_addr);
        ::connect(this->_fd, (struct sockaddr*)&addr, sizeof(addr));
    }

    void Client::send(string param){
        ::send(this->_fd, param.c_str(), param.length(), MSG_NOSIGNAL);
    }

    string Client::result(){
        return this->_result;
    }

    void Client::handleRead(){
        this->_result = "";
        char buf1[1024] = { 0 };
        char buf2[65535] = { 0 };
        struct iovec iov[2];
        ssize_t nread;

        iov[0].iov_base = buf1;
        iov[0].iov_len = sizeof(buf1);
        iov[1].iov_base = buf2;
        iov[1].iov_len = sizeof(buf2);

        nread = ::readv(this->_fd, iov, 2);
        if(nread > 0){
            this->_result += buf1;
            this->_result += buf2;
        }else if(nread == 0){
            this->_dispatcher->del(this->_fd);
            close(this->_fd);
            delete this->_channel;
            delete this;
            return;
        }
        this->_readCallback(this);
    }
    void Client::handleWrite(){
        this->_sendCallback(this);
        this->_dispatcher->edit(EPOLLIN | EPOLLET, this->_fd, (void*) this->_channel);
    }

    void Client::handleClose(){
        cout << "handleClose" << endl;
    }

    void Client::handleError(){
        cout << "handleError" << endl;
    }

    void Client::handleDestroy(){

    }

    int Client::fd(){
        return this->_fd;
    }
}