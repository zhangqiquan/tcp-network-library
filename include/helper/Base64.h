// +----------------------------------------------------------------------
// | zhanshop / Base64.h    [ 2022/12/27 上午10:35 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
//标准库
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
using namespace boost::archive::iterators;
namespace zhanshop {

    class Base64 {
    public:
        /**
         * 编码
         * @param inPut
         * @return
         */
        inline static string encode(const string &inPut) {
            typedef base64_from_binary<transform_width<string::const_iterator,6,8>> Base64EncodeIter;

            stringstream  result;
            copy(Base64EncodeIter(inPut.begin()),
                 Base64EncodeIter(inPut.end()),
                 ostream_iterator<char>(result));

            size_t Num = (3 - inPut.length() % 3) % 3;
            for (size_t i = 0; i < Num; i++)
            {
                result.put('=');
            }
            string outPut;
            result >> outPut;
            return outPut;
        }

        /**
         * 解码
         */
        inline string decode(const string &inPut) {
            typedef transform_width<binary_from_base64<string::const_iterator>,8,6> Base64DecodeIter;

            stringstream result;
            try
            {
                copy(Base64DecodeIter(inPut.begin()),
                     Base64DecodeIter(inPut.end()),
                     ostream_iterator<char>(result));
            }
            catch (...)
            {
                return "";
            }
            string outPut;
            result >> outPut;
            return outPut;
        };
    };

}
