// +----------------------------------------------------------------------
// | zhanshop-tcp2 / Error.h    [ 2022/12/1 上午10:06 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once

// +----------------------------------------------------------------------
// | zhanshop-http / Error.hpp    [ 2022/11/3 09:43 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <sys/stat.h>
#include "Log.h"

namespace zhanshop {

    class Error {
    public:
        static int execCmd(char* cmd, char *res){
            int len = 0;

            if(strlen(cmd) <= 0){
                return len;
            }

            FILE *fp = NULL;
            char buff[2048];
            memset(buff, 0, 2048);

            if( (fp = popen(cmd, "r")) == NULL ){
                return len;
            }


            while( (fgets(buff, sizeof(buff), fp)) != NULL )
            {
                strcat(res, buff);
                if (strlen(res) > 2000){
                    break;
                }
            }

            pclose(fp);

            return strlen(res);
        }

        static void debug_backtrace(){
            void *array[32];
            size_t size;
            char **strings;

            size = backtrace (array, 200);
            strings = backtrace_symbols (array, size);
            if (NULL == strings){
                printf("backtrace_symbols\n");
                return ;
            }

            size_t name_size = 100;
            char *name = (char*)malloc(name_size);
            for (size_t i = 0; i < size; ++i) {
                char *begin_name = 0;
                char *begin_offset = 0;
                char *end_offset = 0;
                for (char *p = strings[i]; *p; ++p) { // 利用了符号信息的格式
                    if (*p == '(') { // 左括号
                        begin_name = p;
                    }
                    else if (*p == '+' && begin_name) { // 地址偏移符号
                        begin_offset = p;
                    }
                    else if (*p == ')' && begin_offset) { // 右括号
                        end_offset = p;
                        break;
                    }
                }
                if (begin_name && begin_offset && end_offset ) {
                    *begin_name++ = '\0';
                    *begin_offset++ = '\0';
                    *end_offset = '\0';
                    int status = -4; // 0 -1 -2 -3
                    char *ret = abi::__cxa_demangle(begin_name, name, &name_size, &status);
                    if (0 == status) {
                        name = ret;
                        printf("%s:%s+%s\n", strings[i], name, begin_offset);
                        char addr2line[1024] = { 0 };
                        sprintf(addr2line, "addr2line ");
                        sprintf(addr2line + strlen(addr2line), "%s", begin_offset);
                        sprintf(addr2line + strlen(addr2line), " -e ");
                        sprintf(addr2line + strlen(addr2line), "%s", strings[i]);
                        char buff[1024] = {0};
                        Error::execCmd(addr2line, buff);
                        printf("\033[31;31m 追踪头: %s \033[0m\n", buff);
                    }
                    else {
                        // strings[i] 文件
                        // begin_name 函数名
                        // begin_offset 偏移量
                        //system();
                        printf("%s:%s()+%s\n", strings[i], begin_name, begin_offset);
                        char addr2line[1024] = { 0 };
                        sprintf(addr2line, "addr2line ");
                        sprintf(addr2line + strlen(addr2line), "%s", begin_offset);
                        sprintf(addr2line + strlen(addr2line), " -e ");
                        sprintf(addr2line + strlen(addr2line), "%s", strings[i]);
                        char buff[1024] = {0};
                        Error::execCmd(addr2line, buff);
                        printf("\033[31;31m 追踪: %s \033[0m\n", buff);
                        //system(addr2line);
                    }
                }
                else {
                    printf("%s\n", strings[i]);
                }
            }
            free(name);

            free(strings);
        }

        static void handler(int signal){
            LOG_ERROR("发生错误了");
            printf("\n");
            printf("\033[31;31m%d 产生错误信号!!! 说明: \033[0m", signal);
            switch (signal) {
                case 2:
                    printf("程序终止信号。当用户按下CRTL+C时通知前台进程组终止进程。");
                    break;
                case 4:
                    printf("执行了非法指令。通常是因为可执行文件本身出现错误，或者数据段、堆栈溢出时也有可能产生这个信号。");
                    break;
                case 6:
                    printf("调用abort函数产生，将会使程序非正常结束。");
                    break;
                case 7:
                    printf("非法地址。包括内存地址对齐出错。比如访问一个4个字长的整数，但其地址不是4的倍数。它与SIGSEGV的区别在于后者是由于对合法地址的非法访问触发。");
                    break;
                case 8:
                    printf("发生致命的算术运算错误。");
                    break;
                case 13:
                    printf("管道破裂信号。当对一个读进程已经运行结束的管道执行写操作时产生。");
                    break;
                case 16:
                    printf("堆栈错误。");
                    break;
                case 23:
                    printf("套接字上出现紧急情况时产生。");
                    break;
                case 24:
                    printf("超过CPU时间资源限制时产生的信号。");
                    break;
                case 25:
                    printf("当进程企图扩大文件以至于超过文件大小资源限制时产生。");
                    break;
                case 30:
                    printf("电源失效信号。");
                    break;
                case 31:
                    printf("非法的系统调用");
                    break;
                case 11:
                    printf("非法地址，非法访问");
                    break;
                case 15:
                    printf("正常结束进程的信号，kill 命令的默认信号");
                    break;
            }
            printf("\n\n\033[33;33m错误详情:\033[0m\n");
            debug_backtrace();
            exit(0);
        }

        static void listen(){
            signal(SIGINT, Error::handler);
            signal(SIGQUIT, Error::handler);
            signal(SIGABRT, Error::handler);
            signal(SIGKILL, Error::handler);
            signal(SIGTERM, Error::handler);
            signal(SIGSEGV, Error::handler);
            signal(SIGPIPE, Error::handler);
        }
    };

}

