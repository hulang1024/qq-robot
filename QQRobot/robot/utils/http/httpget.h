/*
http get请求
author: hulang
*/
#ifndef HTTPGET_H
#define HTTPGET_H

#include <map>
#include "libs/curl/curl.h"
#include "libs/jsoncpp/json.h"

using namespace std;

namespace QQRobot
{
    class HttpGet
    {
    public:
        HttpGet();
        HttpGet(string url);
        ~HttpGet();

        HttpGet& setUrl(string url);

        HttpGet& addParam(string name, string value);
        HttpGet& addParam(string name, int value);
        HttpGet& clearParams();

        /*
        return  如果成功，返回http状态码，否则返回<0值
        */
        int performForString(string &responseString);
        int performForJSON(Json::Value &root);

    private:
        map<string, string> params;
        string url;

        // curl
        CURL *pCurl = NULL;
        CURLcode code;
        bool curlInited = false;

        string getQueryParamString();

        static size_t writeFunc(void *input, size_t uSize, size_t uCount, void *avg);
    };
}

#endif