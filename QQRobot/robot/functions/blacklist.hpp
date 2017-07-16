/*
黑名单功能
author: hulang
*/
#ifndef ROBOT_BLACKLIST_H
#define ROBOT_BLACKLIST_H

#include <string>
#include <map>
#include <vector>
#include "robot/utils/stringutil.h"
#include "function.hpp"

using namespace std;
using namespace QQRobot;
namespace QQRobot
{

    class BlackList : public Function
    {
    public:
        BlackList() {}

        handle_message_code handleMessage(Message &fromMsg, Message &toMsg)
        {
            if (fromMsg.from != robot->masterQQ)
            {
                toMsg.setContent("无权限");
                robot->sender->sendMessage(toMsg);
                return handle_message_code::block;
            }
            vector<string> strs = stringutil::split(fromMsg.getContent(), " ");
            if (strs.size() < 2)
                return handle_message_code::syntax_error;
            string operatorStr = strs[1];

            if (operatorStr == "add" && strs.size() >= 3)
            {
                string argStr = GroupMessage::tryGetQQFromAtContent(strs[2]);
                addQQ(argStr);
                toMsg.setContent(argStr + " 成功关进小黑屋");
            }
            else if (operatorStr == "del" && strs.size() >= 3)
            {
                string argStr = GroupMessage::tryGetQQFromAtContent(strs[2]);
                delQQ(argStr);
                toMsg.setContent(argStr + " 成功从小黑屋释放");
            }
            else if (operatorStr == "list")
            {
                toMsg.setContent(empty() ? "黑名单空" : "黑名单:\n" + toMutilLineStr(" "));
            }
            else if (operatorStr == "clear")
            {
                clear();
                toMsg.setContent("成功清空黑名单");
            }
            else
            {
                return handle_message_code::syntax_error;
            }
            robot->sender->sendMessage(toMsg);
            return handle_message_code::block;
        }

        void addQQ(string qq)
        {
            mmap[qq] = true;
        }

        void delQQ(string qq)
        {
            mmap.erase(qq);
        }

        void clear()
        {
            mmap.clear();
        }
        
        bool exist(string qq)
        {
            return mmap.count(qq) > 0;
        }
        
        bool empty()
        {
            return mmap.size() == 0;
        }

        string toMutilLineStr(char* indent)
        {
            string str;
            for (map<string, bool>::iterator iter = mmap.begin(); iter != mmap.end(); iter++)
                str += indent + iter->first + "\n";
            return str;
        }

    private:
        map<string, bool> mmap;
    };
}

#endif