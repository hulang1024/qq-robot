/*
消息
author: hulang
*/
#ifndef ROBOT_MESSAGE_H
#define ROBOT_MESSAGE_H

#include <string>

/// 消息中的代码
#define code_msg_face(id) (string("[CQ:face,id=") + to_string(id) + "]")
#define code_msg_emoji(id) (string("[CQ:emoji,id=") + to_string(id) + "]")
#define code_msg_at(qq) (string("[CQ:at,qq=") + qq + "]")
#define code_msg_image(file) (string("[CQ:image,file=") + file + "]")

using namespace std;

namespace QQRobot
{
    class Message
    {
    public:
        string from;
        string to;

        void setContent(string content) { this->content = content; }
        virtual string getContent() { return content; }
    protected:
        string content;		/* 消息内容 */
    };
}

#endif