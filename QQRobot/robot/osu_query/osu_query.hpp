#include <string>
#include "../stringutil.hpp"
#include "curl/curl.h"  
#include "jsoncpp/json.h"
using namespace std;

namespace QQRobot
{
	class OsuQuery
	{
        const string apikey = "8d81085d1374ea124c124283fe7612b7cb35dbd8";
	public:
        OsuQuery()
        {
            code = curl_global_init(CURL_GLOBAL_DEFAULT);
            if (code != CURLE_OK) {
                return;
            }
        }
        ~OsuQuery()
        {
            curl_global_cleanup();
        }

		string query(string text)
		{
            size_t startIndex = text.find(" ");
            if (startIndex == string::npos)
                return "";
            string username = text.substr(startIndex + 1);
            return stat(username);
		}

        string stat(string username)
        {
            string body;

            pCurl = curl_easy_init();
            if (pCurl == NULL) {
                return "curl_easy_init failed";
            }
            curl_easy_setopt(pCurl, CURLOPT_URL, "https://osu.ppy.sh/api/get_user?k=" + apikey + "&u=" + username + "&m=0&type=string");  // 访问的URL  
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 20);
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, &WriteFunction);
            curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &body);

            code = curl_easy_perform(pCurl);
            if (code != CURLE_OK) {
                return "curl_easy_perform error";
            }

            long retCode = 0;
            code = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &retCode);
            if (code != CURLE_OK) {
                return "curl_easy_getinfo error";
            }
            if(retCode != 200)
                return "Http Return Code " + retCode;

            Json::Value root;
            Json::Reader reader;

            bool succeed = reader.parse(body, root);
            if (!succeed)
                return "未查询到数据";

            Json::Value user = root[0];
            Json::Value::Members members = user.getMemberNames();

            if (body.length() < 3)
            {
                return "未查询到数据";
            }

            map<string, string> zhchsMap;
            zhchsMap["username"] = "用户名";
            zhchsMap["country"] = "国家";
            zhchsMap["pp_rank"] = "PP #";
            zhchsMap["pp_country_rank"] = "PP国家#";
            zhchsMap["pp_raw"] = "PP";
            zhchsMap["accuracy"] = "ACC";
            zhchsMap["playcount"] = "PC";
            zhchsMap["ranked_score"] = "已排名分数";
            zhchsMap["total_score"] = "总分数";
            zhchsMap["level"] = "当前等级";
            zhchsMap["count300"] = "300个数";
            zhchsMap["count100"] = "100个数";
            zhchsMap["count50"] = "50个数";
            zhchsMap["count_rank_ss"] = "SS个数";
            zhchsMap["count_rank_s"] = "S个数";
            zhchsMap["count_rank_a"] = "A个数";
            zhchsMap["events"] = "事件";
            zhchsMap["nodata"] = "无数据";

            string orderedDisplayKeys[] = {
                "username", "country", "pp_rank", "pp_country_rank", "pp_raw", "accuracy",
                "playcount", "ranked_score", "total_score", "level", "count300", "count300", "count100", "count50",
                "count_rank_ss", "count_rank_s", "count_rank_a", "events" };

            string resultStr;
            for (int i = 0, j = sizeof orderedDisplayKeys / sizeof(string); i < j; i++)
            {
                string key = orderedDisplayKeys[i];
                if (zhchsMap.count(key) > 0)
                {
                    Json::Value val = user[key];
                    string displayVal = val.empty() ? zhchsMap["nodata"] : val.asString();//前提：val是字符串或空数组
                    resultStr += zhchsMap[key] + "：" + displayVal + "\n";
                }
            }

            curl_easy_cleanup(pCurl);

            return resultStr;
        }

    private:
        CURL *pCurl = NULL;
        CURLcode code;

        static size_t WriteFunction(void *input, size_t uSize, size_t uCount, void *avg)
        {
            size_t uLen = uSize*uCount;
            string *pStr = (string *)(avg);
            pStr->append((char *)(input), uLen);

            return uLen;
        }
    };
}