#include <string>
#include "../stringutil.hpp"
#include "curl/curl.h"  
#include "jsoncpp/json.h"
using namespace std;

namespace QQRobot
{
	class OsuQuery
	{
        //API DOC: https://github.com/ppy/osu-api/wiki
        const string apikey = "8d81085d1374ea124c124283fe7612b7cb35dbd8";

	public:
        OsuQuery()
        {
        }
        ~OsuQuery()
        {
            curl_easy_cleanup(pCurl);
            curl_global_cleanup();
        }

        string query(string text)
        {
            size_t uindex = text.find(" ");
            size_t mindex = text.find_last_of(" *");
            if (uindex == string::npos)
                return "";

            string username, modearg;
            int mode = 0;  // 0 = osu!/std, 1 = Taiko, 2 = CTB, 3 = mania

            if (mindex != string::npos)
            {
                modearg = text.substr(mindex);
                username = text.substr(uindex + 1, text.length() - modearg.length() - 5 - 1);
                modearg = modearg.substr(1);

                if (modearg == "0" || modearg == "1" || modearg == "2" || modearg == "3")
                    mode = (int)(modearg[0] - '0');
                else
                {
                    modearg[0] = tolower(modearg[0]);
                    if (modearg.find("o") == 0 || modearg.find("s") == 0)
                        mode = 0;
                    else if (modearg.find("t") == 0)
                        mode = 1;
                    else if (modearg.find("c") != string::npos)
                        mode = 2;
                    else if (modearg.find("m") != string::npos)
                        mode = 3;
                }
            }
            else
            {
                username = text.substr(uindex + 1);
            }

            return stat(username, mode);
        }

        string stat(string username, int mode)
        {

            if (!curlInited)
            {
                code = curl_global_init(CURL_GLOBAL_DEFAULT);
                if (code != CURLE_OK) {
                    return "";
                }
            }

            if (pCurl == NULL)
            {
                pCurl = curl_easy_init();
                if(pCurl == NULL)
                    return "";
            }

            string body;
            string params = "&u=" + username + "&m=" + to_string(mode) + "&type=string";
            curl_easy_setopt(pCurl, CURLOPT_URL, "https://osu.ppy.sh/api/get_user?k=" + apikey + params);  // 访问的URL  
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 20);
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, &WriteFunction);
            curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &body);

            code = curl_easy_perform(pCurl);
            if (code != CURLE_OK) {
                return "";
            }

            long retCode = 0;
            code = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &retCode);
            if (code != CURLE_OK) {
                return "";
            }
            if(retCode != 200)
                return "Http Return Code " + retCode;

            if (body.length() < 3)
            {
                return "未查询到数据";
            }

            Json::Value root;
            Json::Reader reader;

            bool succeed = reader.parse(body, root);
            if (!succeed)
                return "未查询到数据";

            Json::Value user = root[0];
            Json::Value::Members members = user.getMemberNames();

            map<string, string> zhchsMap;
            zhchsMap["username"] = "用户名";
            zhchsMap["country"] = "国家";
            zhchsMap["pp_rank"] = "PP排名";
            zhchsMap["pp_country_rank"] = "PP国家排名";
            zhchsMap["pp_raw"] = "PP";
            zhchsMap["accuracy"] = "Acc";
            zhchsMap["playcount"] = "PC";
            zhchsMap["ranked_score"] = "已排名分数";
            zhchsMap["total_score"] = "总分数";
            zhchsMap["level"] = "当前等级";
            zhchsMap["count300"] = "300数";
            zhchsMap["count100"] = "100数";
            zhchsMap["count50"] = "50数";
            zhchsMap["count_rank_ss"] = "SS数";
            zhchsMap["count_rank_s"] = "S数";
            zhchsMap["count_rank_a"] = "A数";
            //zhchsMap["events"] = "事件";
            zhchsMap["nodata"] = "无数据";
            zhchsMap["mode"] = "模式";

            string orderedDisplayKeys[] = {
                "username", "country", "pp_rank", "pp_country_rank", "pp_raw", "accuracy",
                "playcount", "ranked_score", "total_score", "level", "count300", "count100", "count50",
                "count_rank_ss", "count_rank_s", "count_rank_a", /*"events"*/ };

            string resultStr;
            string modeName[] = { "osu!", "Taiko", "CatchTheBeat", "osu!mania" };
            resultStr += zhchsMap["mode"] + "： " + modeName[mode] + "\n";
            for (int i = 0, j = sizeof orderedDisplayKeys / sizeof(string); i < j; i++)
            {
                string key = orderedDisplayKeys[i];
                if (zhchsMap.count(key) > 0)
                {
                    Json::Value val = user[key];
                    string displayVal = val.empty() ? zhchsMap["nodata"] : val.asString();//前提：val是字符串或空数组
                    resultStr += zhchsMap[key] + "： " + displayVal + "\n";
                }
            }

            return resultStr;
        }

    private:
        CURL *pCurl = NULL;
        CURLcode code;
        bool curlInited = false;

        static size_t WriteFunction(void *input, size_t uSize, size_t uCount, void *avg)
        {
            size_t uLen = uSize*uCount;
            string *pStr = (string *)(avg);
            pStr->append((char *)(input), uLen);

            return uLen;
        }
    };
}