// +----------------------------------------------------------------------
// | zhanshop / Str.h    [ 2022/12/27 上午10:40 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#pragma once
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

namespace zhanshop {

    class Str {
    public:
    /**
     * 删除字符串左右的空白字符
     * @param s
     * @param character
     * @return
     */
    inline string trim(string s, string character = "\r\n\t"){
            size_t n = s.find_last_not_of(character);
            if (n != string::npos){
                s.erase(n + 1, s.size() - n);
            }
            n = s.find_first_not_of(character);
            if (n != string::npos){
                s.erase(0, n);
            }
            return s;
        }
        /**
         * 查找字符串的首次出现返回从查找字符串后面的字符串
         * @param haystack
         * @param needle
         * @return
         */
        inline string strstr(string haystack, string needle){
            return ::strstr(haystack.c_str(), needle.c_str());
        }

        /**
         * 查找字符串首次出现的位置
         * @param haystack
         * @param needle
         * @return
         */
        inline int strpos(string* haystack, string needle, int offset = 0){
            const char *p = ::strstr(haystack->c_str(), needle.c_str());
            if(p)
            {
                return p - haystack->c_str();
            }
            return -1;
        }

        /**
         * 统计字符串的字节数，而不是其中字符的数量
         * @param str
         * @return
         */
        inline int strlen(const string *str){
            return ::strlen(str->c_str());
        }
        /**
         * 把字符串分割到数组中
         * @param str
         * @param length
         * @return
         */
        inline vector<string> strsplit(const string& str, int length){
            vector<string> res;
            int key = 0;
            string val = "";
            for(int i = 0; i < ::strlen(str.c_str()); i++){
                if(key >= length){
                    res.push_back(val);
                    val = "";
                    key = 0;
                }
                val += str[i];
                key++;
            }
            if(val != "") res.push_back(val);
            return res;
        }

        inline vector<string> stringsplit(const string& str, const string& split)
        {
            vector<string> res;
            char* strc = new char[str.size() + 1];
            strcpy(strc, str.c_str());   // 将str拷贝到 char类型的strc中
            char* temp = strtok(strc, split.c_str());
            while (temp != NULL)
            {
                res.push_back(string(temp));
                temp = strtok(NULL, split.c_str());	// 下一个被分割的串
            }

            delete[] strc;
            return res;
        }
        /**
         * 使用一个字符串分割另一个字符串[字符串拆分数组]
         * @param separator
         * @param str
         * @return
         */
        inline std::vector<std::string> explode(const std::string &separator, const std::string &str)
        {
            std::vector<std::string> resVec;

            if ("" == str)
            {
                return resVec;
            }
            //方便截取最后一段数据
            std::string strs = str + separator;

            size_t pos = strs.find(separator);
            size_t size = strs.size();

            while (pos != std::string::npos)
            {
                std::string x = strs.substr(0,pos);
                x.erase(0,x.find_first_not_of(" "));
                x.erase(x.find_last_not_of(" ") + 1);
                resVec.push_back(Str::trim(x, "\t\r"));
                strs = strs.substr(pos+1,size);
                pos = strs.find(separator);
            }

            return resVec;
        }
        /**
         * 查找指定字符在字符串中的最后一次出现
         * @param haystack
         * @param needle
         * @return
         */
        inline int strrchr(string* haystack, char needle){
            const char * ch = ::strrchr(haystack->c_str(), needle);
            return ch - haystack->c_str() + 1;
        }
        /**
         * 字符串替换
         * @param search
         * @param replace
         * @param subject
         * @return
         */
        inline string& str_replace(const string &search, const string &replace, string &subject)
        {
            string buffer;

            int sealeng = search.length();
            int strleng = subject.length();

            if (sealeng==0)
                return subject;//no change

            for(int i = 0, j = 0; i < strleng; j = 0 )
            {
                while (i + j < strleng && j < sealeng && subject[i + j] == search[j])
                    j++;
                if (j == sealeng)//found 'search'
                {
                    buffer.append(replace);
                    i += sealeng;
                }
                else
                {
                    buffer.append( &subject[i++], 1);
                }
            }
            subject = buffer;
            return subject;
        }

        /**
         * 查找指定字符在字符串中的最后一次出现位置【不支持中文】返回从查找字符串后面的字符串
         * @param haystack
         * @param needle
         * @return
         */
        // strrchr("zhangqiquandediannao", 'de')

        /**
         * 字符串截取
         * @param str
         * @param offset
         * @param length
         * @return
         */
        inline string substr(string str, int offset, int length){
            return str.substr(offset, length);
        }
        /**
         * 转大写
         * @param str
         * @return
         */
        inline string toupper(string str){
            transform(str.begin(),str.end(),str.begin(),::toupper);
            return str;
        }
        /**
         * 字符串传小写
         * @param str
         * @return
         */
        inline string tolower(string str){
            transform(str.begin(),str.end(),str.begin(),::tolower);
            return str;
        }
    };

}
