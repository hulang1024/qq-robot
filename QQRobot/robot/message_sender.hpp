#include "group_message.hpp"
#include "cqp.h"

using namespace QQRobot;

namespace QQRobot
{
    class MessageSender
    {
    public:
        void sendMessageToGroup(GroupMessage msg)
        {
            CQ_sendGroupMsg(authCode, msg.toGroupQQ, msg.getContent().c_str());
        }

        void setAuthCode(int authCode)
        {
            this->authCode = authCode;
        }

    private:
        int authCode;
    };
}