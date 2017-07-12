/*
·¢ËÍÏûÏ¢
author: hulang
*/
#ifndef ROBOT_MESSAGE_SENDER_H
#define ROBOT_MESSAGE_SENDER_H

#include <typeinfo>
#include "message.hpp"
#include "group_message.hpp"
#include "private_message.hpp"


using namespace QQRobot;

namespace QQRobot
{
    class MessageSender
    {
    public:
        void sendMessage(Message &msg)
        {
            const type_info& msgType = typeid(msg);

            if (msgType == typeid(GroupMessage))
                sendGroupMessage((GroupMessage&)msg);
            else if (msgType == typeid(PrivateMessage))
                sendPrivateMessage((PrivateMessage&)msg);
        }

        void sendGroupMessage(GroupMessage &msg)
        {
            if (msg.type == 0)
                CQ_sendGroupMsg(authCode, atoi(msg.to.c_str()), msg.getContent().c_str());
            else if (msg.type == 1)
                CQ_sendDiscussMsg(authCode, atoi(msg.to.c_str()), msg.getContent().c_str());
        }

        void sendPrivateMessage(PrivateMessage &msg)
        {
            CQ_sendPrivateMsg(authCode, atoi(msg.to.c_str()), msg.getContent().c_str());
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