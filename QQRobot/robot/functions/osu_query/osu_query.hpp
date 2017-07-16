/*
osu!查询功能
author: hulang
*/
#ifndef OSU_QUERY_H
#define OSU_QUERY_H

#include <string>
#include "robot/utils/http/httpget.h"  
#include "libs/jsoncpp/json.h"
#include "robot/functions/function.hpp"

using namespace std;
using namespace QQRobot;

namespace QQRobot
{
    class OsuQuery : public Function
    {
        //API DOC: https://github.com/ppy/osu-api/wiki
        const string apikey = "8d81085d1374ea124c124283fe7612b7cb35dbd8";

    public:
        OsuQuery() {}

        handle_message_code handleMessage(Message &fromMsg, Message &toMsg)
        {
            return query(fromMsg, toMsg);
        }

    private:
        handle_message_code query(Message &fromMsg, Message &toMsg)
        {
            string text = fromMsg.getContent();
            size_t uindex = text.find(" ");
            size_t mindex = text.find_last_of("*");
            if (uindex == string::npos)
                return handle_message_code::syntax_error;

            string username, modearg;
            int mode = 0;  // 0 = osu!/std, 1 = Taiko, 2 = CTB, 3 = mania

            if (mindex != string::npos)
            {
                modearg = text.substr(mindex);
                username = text.substr(uindex + 1, text.length() - modearg.length() - 5 - 1 - 1);
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

            string result = stat(username, mode);
            if (result.length() > 0)
            {
                toMsg.setContent(result);
                robot->sender->sendMessage(toMsg);
                return handle_message_code::block;
            }
            return handle_message_code::ignore;
        }

        string stat(string username, int mode)
        {
            Json::Value root;
            get.setUrl("https://osu.ppy.sh/api/get_user")
                .addParam("u", username)
                .addParam("m", to_string(mode))
                .addParam("type", "string")
                .addParam("k", apikey)
                .performForJSON(root);

            Json::Value user = root[0];

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
            HttpGet get = HttpGet();
    };
}

#endif