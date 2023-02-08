// +----------------------------------------------------------------------
// | zhanshop / HttpContext.cc    [ 2022/12/17 下午4:52 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2011~2022 zhangqiquan All rights reserved.
// +----------------------------------------------------------------------
// | Author: zhangqiquan <768617998@qq.com>
// +----------------------------------------------------------------------
#include "../../include/http/HttpContext.h"

namespace zhanshop {
    HttpContext::HttpContext():_httpRequest(make_unique<HttpRequest>()){

    }

    HttpRequest* HttpContext::httpRequest(){
        return this->_httpRequest.get();
    }

    string& HttpContext::protocol(){
        return this->_protocol;
    }

    void HttpContext::setProtocol(string protocol){
        this->_protocol = protocol;
    }

    bool HttpContext::parseRequest(string& buf){
        if(buf.length() <= 0){
            return false;
        }
        int position = buf.find_first_of("\r\n");
        string  reqLine = buf.substr(0, position);

        int methodPos =  reqLine.find_first_of(" ");
        int urlPos =  reqLine.find(" ", methodPos + 1);

        string path =  reqLine.substr(methodPos + 1, urlPos- methodPos - 1);
        int queryPos = path.find_first_of("?");

        this->_httpRequest->setMethod(reqLine.substr(0, methodPos));
        this->_httpRequest->setPath(path.substr(0, queryPos));
        this->_httpRequest->setQuery(path.substr(queryPos + 1, path.length() - queryPos - 1));
        this->_httpRequest->setVersion( reqLine.substr(urlPos + 1,  reqLine.length() - urlPos - 1));
        buf.erase(0, position + 2);
        this->parseHeader(buf);
        return true;
    }

    void HttpContext::parseHeader(string& buf){
        int start = 0; // 开始搜索的位置
        int endPos = 0; // 当前行的结束位置
        int sumNUll = 0; // 空的总数
        for(;;){
            int position = buf.find("\r\n", start); // 搞出来了一个-1 拿到一行header的结束位置
            if(position == -1){
                break;
            }else{
                endPos = position;
            }
            string headerLine = buf.substr(start, endPos - start); // 拿到一行header
            int keyPos = headerLine.find_first_of(": "); // 这里必须得有两个值的情况下
            if(keyPos > 0){
                this->_httpRequest->setHeader(headerLine.substr(0, keyPos), headerLine.substr(keyPos + 2, headerLine.length() - keyPos - 2));
            }

            start = endPos + 2;
            if(headerLine == ""){
                ++sumNUll;
            }
            if(sumNUll >= 2){
                break;
            }
        }
        endPos += 2;
        string contentLength = this->_httpRequest->header("Content-Length");
        if(contentLength != ""){
            int length = atoi(contentLength.c_str());
            if(length > 0){
                endPos += length;
                this->_httpRequest->setBody(buf.substr(start, length));
            }
        }
        // 擦除
        buf.erase(0, endPos);
    }

    int HttpContext::websocketDecode(const char *buf, int len, char* decoded){
//        cout << "###需要解码的数据 : " << buf  << " ; 需要解码的数据长度: " << len << endl;
//
//        cout << "FIN/是否最后一帧：" << (((buf[0] & 128) == 128) ? 1 : 0) << endl;
//        cout << "RSV1：" << ((buf[0] & 64) == 64 ? 1 : 0) << "; 如果不为0，则标记为连接失败" << endl;
//        cout << "RSV2：" << ((buf[0] & 32) == 32 ? 1 : 0) << "; 如果不为0，则标记为连接失败" <<endl;
//        cout << "RSV3：" << ((buf[0] & 16) == 16 ? 1 : 0) << "; 如果不为0，则标记为连接失败" <<endl;
//        cout << "opcode/操作类型：" << int((uint8_t)((int(buf[0]) << 4)) >> 4)  << endl;

        // 暂时仅支持text解码
        if(int((uint8_t)((int(buf[0]) << 4)) >> 4) != 1) return -1;
        int mask = ((buf[1] & 128) == 128 ? 1 : 0);
        if(mask == 0) return -1; // 必须要掩码

        //cout << "Mask/是否需要用掩码进行异或：" << ((buf[1] & 128) == 128 ? 1 : 0) << endl;

        int playload = int((uint8_t)((int(buf[1]) << 1)) >> 1); // 这样会多搞出来一个0
        //cout << "Payload len/有效长度" << playload << endl;

        //memset(decoded, 0, sizeof(decoded));
//
//    return "";

        if(playload <= 125){
            string masks = ""; // 这样肯定是有bug的
            //string decoded = "";

            for (int index = 2; index < 6; index++) {
                masks += buf[index];
            }


            //cout << "使用循环截取："<< masks << endl;

            // 获取到内容了
            for (int andex = 0; andex < playload; andex++) {
                decoded[andex] = buf[andex + 6] ^ masks[andex % 4];
                //decoded += buf[andex + 6] ^ masks[andex % 4];
            }
            //cout << "ok解码出来的数据" << decoded << endl;
            return 0;
        }else if(playload == 126){

            uint16_t payloadLength16b = 0;

            // 126的时候
            memcpy(&payloadLength16b, &buf[2], 2); // 相当于拿了两个char
            int payloadLength = ntohs(payloadLength16b);
            // 拿掩码
            string masks = "";
            for (int index = 0; index < 4; index++) {
                masks[index] = buf[index+4];
            }

            //cout << "真实数据长度：" << payloadLength << endl;
            //string decoded = "";

//        // 获取到内容了
            for (int andex = 0; andex < payloadLength; andex++) {
                decoded[andex] = buf[andex + 8] ^ masks[andex % 4];
                //decoded += buf[andex+8] ^ masks[andex % 4];
            }
            //cout << "ok解码出来的数据: " << decoded << endl;
            return 0;
        }
        return -1;
    }
}