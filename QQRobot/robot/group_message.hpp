#ifndef ROBOT_GROUP_MESSAGE_H
#define ROBOT_GROUP_MESSAGE_H

#include <string>
#include "message.hpp"

using namespace std;
using namespace QQRobot;

namespace QQRobot
{
    class GroupMessage : public Message
    {
    public:
        string groupQQ;
        string atQQ = "";
        int type = 0; // 0=群，1=讨论组

        string getContent()
        {
            if (atQQ == "")
                return content;
            return "[CQ:at,qq=" + atQQ + "] "+ content;
        }

        void setAtQQ(string qq)
        {
            atQQ = qq;
        }

        void delAtQQ(string qq)
        {
            atQQ = -1;
        }

        string getAtQQ()
        {
            if (atQQ != "")
                return atQQ;

            int i = content.find("[CQ:at,qq=");
            if (i != string::npos)
            {
                int j = content.find("]");
                if (j != string::npos)
                    return content.substr(i + 10, j - i - 10);
            }

            return "";
        }

        static string tryGetQQFromAtContent(string content)
        {
            int i = content.find("[CQ:at,qq=");
            if (i != string::npos)
            {
                int j = content.find("]");
                if (j != string::npos)
                    return content.substr(i + 10, j - i - 10);
            }
            return content;
		}

        string atContent()
        {
            int i = content.find("[CQ:at,qq=");
            if (i != string::npos)
            {
                int j = content.find("]");
                if (j != string::npos)
                    return content.substr(0, i) + content.substr(j + 1);
            }
            return content;
        }
    };
}

#endif