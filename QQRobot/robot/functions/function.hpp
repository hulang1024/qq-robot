#ifndef ROBOT_FUNCTION_H
#define ROBOT_FUNCTION_H

#include <string>
#include "../message.hpp"
#include "../message_sender.hpp"

using namespace std;
using namespace QQRobot;

namespace QQRobot
{
    /*
    机器人功能抽象
    */
    class Function
    {
    public:
        MessageSender *sender;
        Robot *robot;

        Function() {}
        Function(MessageSender *sender, Robot *robot)
        {
            this->sender = sender;
            this->robot = robot;
        }

        /*
        处理消息
        return: 是否不忽略
        */
        virtual bool handleMessage(Message &fromMsg, Message &toMsg) = 0;

        static string functionInfo()
        {
            string info = "我的功能如下：\n";
            info += "  * 我可以执行JS程序，发送：eval: <JS代码>`，例如：eval: 1+2。注意分号是英文的，分号后面可任意空白。\n";
            info += "  * 如果你@我，我也会@你。\n";
            info += "  * osu!查询: 1. !stat\n";
            info += "  * 命令用法查询: !man <命令名>，例如：!man stat";
            return info;
        }
    };
}

#endif // ! FUNCTION_H
