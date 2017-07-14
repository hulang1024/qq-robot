/*
私聊消息
author: hulang
*/
#ifndef ROBOT_PRIVATE_MESSAGE_H
#define ROBOT_PRIVATE_MESSAGE_H

#include "utils/stringutil.h"
#include "message.hpp"

using namespace std;
using namespace QQRobot;

namespace QQRobot
{
    enum MessageSubType
    {
        FromFriend = 11,
        FromOnline = 1,
        FromGroup = 2,
        FromDiscuss = 3
    };

    class PrivateMessage : public Message
    {
    public:
        MessageSubType subType;

        string getContent() { return content; }
    };
}

#endif