#ifndef QQROBOT_MESSAGE_H
#define QQROBOT_MESSAGE_H

#include <string>
using namespace std;

namespace QQRobot
{
    enum MessageSubType
    {
        FromFriend = 11,
        FromOnline = 1,
        FromGroup = 2,
        FromDiscuss = 3
    };

    class Message
    {
    public:
        string from;
        string to;
        MessageSubType subType;

        void setContent(string content) { this->content = content; }
        string getContent() { return content; }
    protected:
        string content;		/* ÏûÏ¢ÄÚÈİ */
    };
}

#endif