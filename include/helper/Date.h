// +----------------------------------------------------------------------
// | zhanshop / Date.h    [ 2022/12/1 下午2:50 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <string>
#include <sys/time.h>

using namespace std;

namespace zhanshop {

    namespace Date {

        inline string date(){
            time_t t = ::time(NULL); // 当前时间戳

            char buff[80];
            auto timeinfo = localtime(&t);
            strftime(buff, 64, "%Y-%m-%d %H:%M:%S", timeinfo);
            return (string) buff;
        }

        inline string date(time_t t){
            char buff[80];
            auto timeinfo = localtime(&t);
            strftime(buff, 64, "%Y-%m-%d %H:%M:%S", timeinfo);
            return (string) buff;
        }

        inline double microtime(){
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            return (double)tv.tv_sec + (tv.tv_usec / (double) 1000000);
        }

    }
}

