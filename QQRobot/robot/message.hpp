#ifndef ROBOT_MESSAGE_H
#define ROBOT_MESSAGE_H

#include <string>
using namespace std;

namespace QQRobot
{
    class Message
    {
    public:
        string from;
        string to;

        void setContent(string content) { this->content = content; }
        string getContent() { return content; }
    protected:
        string content;		/* 消息内容 */
    };
}

#endif