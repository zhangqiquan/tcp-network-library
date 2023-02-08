# 项目描述：
该项目是一个多线程C++网络库，基于反应器模式。，采用非阻塞 IO 模型，基于时间驱动和回调，原生支持多核多线程，适合编写
Linux 服务端多线程网络应用程序, 封装支持有 tcp 协议，http 协议，websocket 协议，以及 tcp 客户端，定时任务等。

# 要求：

Linux内核版本>=5

BOOST库1.75以上

GCC、G++ >=11

# 功能概述：
1. Reactor 主线程 MainReactor 对象通过 epoll 监听连接事件，收到事件后，通过 Acceptor 处理连接事件
2. 当 Acceptor 接受连接事件后，MainReactor 将连接分配给 SubReactor
3. SubReactor 将连接加入子 epoll 进行监听，并创建 Handler 进行各种事件处理
4. 当有新事件发生时，SubReactor 就会调用对应的 Handler 处理 handler
5. handler 通过 read 读取数据，进行分包和响应结果 send 处理
6. 利用 epoll 管理所有的连接、定时器、tcp 客户端请求等文件描述符，全程非阻塞回调。
7. 利用环形队列 boost/circular_buffer + 智能指针的方式 定时让智能指针脱离作用域而剔除超时不通讯的连接。
8. 基于信号 捕获记录全部错误通过 addr2line 解析发生错误的具体内容和行数
9. 封装了对 tcp 协议，http 协议的分包处理，websocket 协议规范的解码掩码等处理, 以及 hashmap 路由名称和类函数绑定的控
   制器路由的支持
