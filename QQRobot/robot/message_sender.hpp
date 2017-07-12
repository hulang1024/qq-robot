#ifndef ROBOT_MESSAGE_SENDER_H
#define ROBOT_MESSAGE_SENDER_H

#include <typeinfo>  
#include "group_message.hpp"
#include "private_message.hpp"
#include "libs/cqp.h"

using namespace QQRobot;

namespace QQRobot
{
    class MessageSender
    {
    public:
        void sendPrivateMessage(PrivateMessage &msg)
        {
            CQ_sendPrivateMsg(authCode, atoi(msg.to.c_str()), msg.getContent().c_str());
        }

        void sendGroupMessage(GroupMessage &msg)
        {
            if(msg.type == 0)
                CQ_sendGroupMsg(authCode, atoi(msg.to.c_str()), msg.getContent().c_str());
            else if(msg.type == 1)
                CQ_sendDiscussMsg(authCode, atoi(msg.to.c_str()), msg.getContent().c_str());
        }

        void sendMessage(Message &msg)
        {
            const type_info& type = typeid(msg);
            if (type == typeid(GroupMessage))
                sendGroupMessage((GroupMessage&)msg);
            else if (type == typeid(GroupMessage))
                sendPrivateMessage((PrivateMessage&)msg);
        }

        void setAuthCode(int authCode)
        {
            this->authCode = authCode;
        }

    private:
        int authCode;
    };
}

#endif