/*
http get请求实现
author: hulang
*/
#include <map>
#include "httpget.h"
#include "robot/utils/stringutil.h"

using namespace std;
using namespace QQRobot;

HttpGet::HttpGet()
{
    if (!curlInited)
    {
        curlInited = true;
        code = curl_global_init(CURL_GLOBAL_DEFAULT);
        if (code != CURLE_OK)
        {
            curlInited = false;
            pCurl = NULL;
        }
        if (pCurl == NULL)
        {
            pCurl = curl_easy_init();
        }
    }
}

HttpGet::HttpGet(string url)
{
    HttpGet();
    setUrl(url);
}

HttpGet::~HttpGet()
{
    curl_easy_cleanup(pCurl);
    curl_global_cleanup();
}

HttpGet& HttpGet::setUrl(string url)
{
    this->url = url;
    return *this;
}

HttpGet& HttpGet::addParam(string name, string value)
{
    params[name] = value;
    return *this;
}
HttpGet& HttpGet::addParam(string name, int value) { return addParam(name, to_string(value)); }

HttpGet& HttpGet::clearParams()
{
    params.clear();
    return *this;
}

int HttpGet::performForString(string &responseString)
{
    if (pCurl == NULL)
        return -1;

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type:text/html;charset=UTF-8;");
    curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(pCurl, CURLOPT_URL, url + "?" + getQueryParamString()); //输入参数url后面应不带'?'
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 100);
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, &writeFunc);
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &responseString);

    code = curl_easy_perform(pCurl);

    curl_slist_free_all(headers);

    if (code != CURLE_OK)
        return -1;

    responseString = stringutil::UTF8_To_string(responseString);

    int resCode = 0;
    code = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &resCode);
    if (code != CURLE_OK)
        return -1;
    return resCode;
}

int HttpGet::performForJSON(Json::Value &root)
{
    string reponseString;
    int ret = performForString(reponseString);
    if (ret < 0)
        return ret;

    Json::Reader reader;

    bool succeed = reader.parse(reponseString, root);
    return succeed ? ret : -1;
}

string HttpGet::getQueryParamString()
{
    if (params.size() == 0)
        return "";
    string str;
    for (map<string, string>::iterator iter = params.begin(); iter != params.end(); iter++)
        str += iter->first + "=" + iter->second + "&";
    return str.substr(0, str.length() - 1);
}

size_t HttpGet::writeFunc(void *input, size_t uSize, size_t uCount, void *avg)
{
    size_t uLen = uSize*uCount;
    string *pStr = (string *)(avg);
    pStr->append((char *)(input), uLen);

    return uLen;
}
