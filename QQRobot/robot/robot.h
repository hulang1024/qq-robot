/*
机器人，单例
author: hulang
*/
#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <map>
#include "libs/cqp.h"
#include "message.hpp"
#include "private_message.hpp"
#include "group_message.hpp"
#include "message_sender.hpp"

using namespace std;
using namespace QQRobot;

namespace QQRobot
{
    class Robot
    {
    public:
        string qq;
        string masterQQ;
        string nickname;

        map<string, void*> solts;//槽

        MessageSender *sender;

        ~Robot();

        static Robot* initance();

        CQ_EVENT_RET onPrivateMessage(PrivateMessage &fromMsg);
        CQ_EVENT_RET onGroupMessage(GroupMessage &fromMsg);
        CQ_EVENT_RET onDiscussMessage(GroupMessage &fromMsg);

    private:
        Robot();
    };
}

#endif