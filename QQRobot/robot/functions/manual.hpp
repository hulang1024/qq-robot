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
            manInfoMap["stat"] = statInfo;
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
                toMsg.setContent(manInfoMap[cmd]);
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