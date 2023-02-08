// +----------------------------------------------------------------------
// | zhanshop / index.cc    [ 2022/12/15 14:20 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <thread>

using namespace std;


void request(){
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6202);   // 大端端口
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("connect");
        exit(0);
    }

    // 3. 和服务器端通信

    for(int i = 0; i < 100; ++i){
        // 发送数据
        string reqData = "";
        reqData += "POST /index.php/aas/q2112?a=2 HTTP/1.1\r\n";
        reqData += "Host: 36.111.20.204:80\r\n";
        reqData += "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:106.0) Gecko/20100101 Firefox/106.0\r\n";
        reqData += "Accept: */*\r\n";
        reqData += "Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2\r\n";
        reqData += "Accept-Encoding: gzip, deflate\r\n";
        reqData += "Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n";
        reqData += "Content-Length: 24\r\n";
        reqData += "Origin: http://127.0.0.1:80\r\n";
        reqData += "Connection: close\r\n";
        reqData += "Referer: http://127.0.0.1:80/\r\n";
        reqData += "\r\n\r\n"; // 这个是分界点
        reqData += "code=6dky&title=6666666"+ to_string(i);
        write(fd, reqData.c_str(), reqData.length()+1);
        //sleep(1); // 测试服务器端是否保活
        cout << "发送消息" << endl;
        close(fd);
    }

//    for(int i = 0; i < 10; ++i){
//        char buf[10240];
//        memset(buf, 0, sizeof(buf));
//        int len = ::recv(fd, buf, sizeof(buf), MSG_NOSIGNAL);
//        if(len == 0){
//            cout << "服务端断开了连接" << endl;
//            break;
//        }else{
//            cout << "接收到服务器端发来的数据：" << buf << endl;
//        }
//    }

    close(fd);
}

int main()
{
    for(unsigned int i = 0; i < 50; ++i){
        thread t(request);
        t.detach();
    }

    for (;;) {

    }

    return 0;
}
