// +----------------------------------------------------------------------
// | tcp2 / Log.h    [ 2022/11/28 16:34 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <iostream>
#include "helper/Date.h"

using namespace std;
using namespace zhanshop;

namespace zhanshop {

#define LOG_INFO(x) \
cout << Date::date() << " [INFO] " << x << "   " << __FILE__ << ":" << __LINE__ << endl;

#define LOG_DEBUG(x) \
cout << Date::date() << " [DEBUG] " << x << "   " << __FILE__ << ":" << __LINE__ << endl;

#define LOG_WARN(x) \
cout << Date::date() << " [WARN] "  << x << "   " << __FILE__ << ":" << __LINE__ << endl;

#define LOG_ERROR(x) \
cout << Date::date() << " [ERROR] "  << x << "   " << __FILE__ << ":" << __LINE__ << endl;

#define LOG_FATAL(x) \
cerr << Date::date() << " [FATAL] " << x << "   " << __FILE__ << ":" << __LINE__ << endl; \
exit(-1);

}
