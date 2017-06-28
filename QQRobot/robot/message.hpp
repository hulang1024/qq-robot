#ifndef QQROBOT_MESSAGE_H
#define QQROBOT_MESSAGE_H

#include <string>
using namespace std;

namespace QQRobot
{
    typedef int64_t QQNumber;

    class Message
    {
    public:
        QQNumber fromQQ;	/* 来自QQ用户 */
        QQNumber toQQ;		/* 目标QQ用户 */

        void setContent(string content) { this->content = content; }
        string getContent() { return content; }
    protected:
        string content;		/* 消息内容 */
    };
}

#endif