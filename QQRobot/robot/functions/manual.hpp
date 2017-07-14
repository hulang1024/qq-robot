/*
手册功能
author: hulang
*/
#ifndef ROBOT_MANUAL_H
#define ROBOT_MANUAL_H

#include <string>
#include <map>
#include "robot/utils/stringutil.h"
#include "function.hpp"

using namespace std;
using namespace QQRobot;

namespace QQRobot
{
    class Robot;

    class Manual : public Function
    {
    public:
        Manual()
        {
            // 初始化命令手册信息
            string statInfo;
            statInfo = "查询osu!用户统计信息。\n";
            statInfo = statInfo
                + "!stat <用户名> *[模式]\n"
                + "模式：0 = osu!/std, 1 = Taiko, 2 = CTB, 3 = osu!mania/mania"
                + "，该参数是可选的，默认为0；可以填数字或相应英文名或英文名字母\n"
                + "例如，查询ctb统计信息：!stat WubWoofWolf *c";
            manInfoMap["!stat"] = statInfo;

            manInfoMap["!black"] = "!black add <qq>\n!black del <qq>\n!black clear\n!black list\n";

            manInfoMap["eval"] = "eval:<JS代码>，例如：eval: 1+2。注意分号是英文的，分号后面可任意空白。";

            manInfoMap["天气"] = "天气 <城市>，例如：天气 武汉";

            manInfoMap["!sendtogroup"] = "!sendtogroup 群号 [可选的QQ号] 消息";
            manInfoMap["!send"] = "!send QQ号 消息";

            manInfoMap["man"] = "命令用法查询: !man <命令名>，例如：!man stat";
        }

        Manual(Robot *robot) : Function(robot)
        {
            Manual();
        }

        bool handleMessage(Message &fromMsg, Message &toMsg)
        {
            vector<string> strs = stringutil::split(fromMsg.getContent(), " ");
            if (strs.size() > 1)
            {
                string cmd = strs[1];
                toMsg.setContent(cmd + "用法信息:\n" + manInfoMap[cmd]);
                robot->sender->sendMessage(toMsg);
                return true;
            }
            return false;
        }


    private:
        map<string, string> manInfoMap;

    };
}

#endif