/*
机器人，监听消息事件
author: hulang
*/
#ifndef ROBOT_H
#define ROBOT_H

#include <string.h>
#include "../libs/cqp.h"
#include "message.hpp"
#include "private_message.hpp"
#include "group_message.hpp"
#include "message_sender.hpp"

using namespace QQRobot;

namespace QQRobot
{
    class Function;
    class BlackList;
    class OsuQuery;
    class Interpreter;
    class Manual;

    class Robot
    {
    public:
        string qq;
        string masterQQ;

        BlackList *blacklist;
        OsuQuery *osuQuery;
        Interpreter *interpreter;
        Manual *man;

        MessageSender *sender;
        
        Robot();
        Robot(MessageSender *sender);
        ~Robot();

        CQ_EVENT_RET onPrivateMessage(PrivateMessage &fromMsg);
        CQ_EVENT_RET onGroupMessage(GroupMessage &fromMsg);
        CQ_EVENT_RET onDiscussMessage(GroupMessage &fromMsg);

    private:
        bool checkIsInBlackList(GroupMessage &fromMsg, GroupMessage &toMsg);
    };
}

#endif