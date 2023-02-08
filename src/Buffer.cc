// +----------------------------------------------------------------------
// | tcp2 / Buffer.cc    [ 2022/11/28 16:32 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../include/Buffer.h"
#include "../include/Socket.h"

namespace zhanshop {
    Buffer::Buffer(int fd):_fd(fd){

    }

    int Buffer::read(){
        memset(this->_readData, 0, sizeof(this->_readData));
        this->_readLen = recv(this->_fd, this->_readData, sizeof(this->_readData), 0);
        return this->_readLen;
    }

//    int Buffer::read(){
//        memset(this->_readData, 0, sizeof(this->_readData));
//        this->_readLen = 0;
//        char buf1[1024] = { 0 };
//        char buf2[65535] = { 0 };
//        struct iovec iov[2];
//
//        iov[0].iov_base = buf1;
//        iov[0].iov_len = sizeof(buf1);
//        iov[1].iov_base = buf2;
//        iov[1].iov_len = sizeof(buf2);
//
//        ssize_t length = ::readv(this->_fd, iov, 2);
//        cout << "消息长度：" << length << endl;
//        if(length > 0){
//            //this->_readData = buf1;
//            memcpy(this->_readData, buf1, strlen(buf1));
//            this->_readLen = strlen(buf1);
//            //memcpy(this->_readData, buf1, strlen(buf1));
//            if(length > 1024){
//                memcpy(this->_readData + strlen(this->_readData), buf2, strlen(buf2));
//            }
//
//            //sprintf(this->_readData, buf1); // 还是这个的问题
////            if(length > 1024){
////                sprintf(this->_readData + strlen(this->_readData), buf2);
////            }
//        }
//        //this->_readLen = (int)length;
//        return this->_readLen;
//    }

    int Buffer::send(string msg){
        return ::send(this->_fd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
    }

    char* Buffer::readData(){
        return this->_readData;
    }

    int& Buffer::readLen(){
        return this->_readLen;
    }

    int& Buffer::fd(){
        return this->_fd;
    }
}