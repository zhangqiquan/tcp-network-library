// +----------------------------------------------------------------------
// | zhanshop / Encrypt.h    [ 2022/12/27 上午10:37 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <iostream>
#include <sstream>
//#include <openssl/aes.h>
//#include <openssl/md5.h>
#include <boost/algorithm/hex.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>
/*  @brief
    SHA1摘要算法:一种很重要的密码学算法,可将任意长度的文本压缩成20个字节的
    独一无二的的摘要(uuid名字生成器使用该算法)
*/
using namespace boost::uuids::detail;

using namespace std;
namespace zhanshop {

    class Encrypt {
    public:
        /**
         * sha1加密[返回的是大写的]
         * @param data
         * @return
         */
        inline static string sha1(std::string buffer, bool binary = false){
            size_t buffer_size = buffer.length();
            char hash[20];
            boost::uuids::detail::sha1 boost_sha1;
            boost_sha1.process_bytes(buffer.c_str(), buffer_size);
            boost::uuids::detail::sha1::digest_type digest;
            boost_sha1.get_digest(digest);
            for(int i = 0; i < 5; ++i) {
                const char *tmp = reinterpret_cast<char*>(digest);
                hash[i*4] = tmp[i*4+3];
                hash[i*4+1] = tmp[i*4+2];
                hash[i*4+2] = tmp[i*4+1];
                hash[i*4+3] = tmp[i*4];
            }

            std::ostringstream buf;
            for(int i = 0; i < 20; ++i) {
                if(binary){
                    buf << hash[i];
                }else{
                    buf << setiosflags(ios::uppercase) << std::hex << ((hash[i] & 0x0000000F0) >> 4);
                    buf << setiosflags(ios::uppercase) << std::hex << (hash[i] & 0x00000000F);
                }
            }

            return buf.str();
        }
    };

}
