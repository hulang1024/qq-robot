#ifndef QQROBOT_GROUP_MESSAGE_H
#define QQROBOT_GROUP_MESSAGE_H

#include <string>
#include "message.hpp"

using namespace std;
using namespace QQRobot;

namespace QQRobot
{
    class GroupMessage : public Message
    {
    public:
        QQNumber fromGroupQQ;   /* 当接收来组的消息时有值 */
        QQNumber toGroupQQ;     /* 当发送到组的消息时有值 */
        QQNumber atQQ = -1;

        string getContent()
        {
            if (atQQ == -1)
                return content;
            return "[CQ:at,qq=" + to_string(atQQ) + "] "+ content;
        }

        void setAtQQ(QQNumber qq)
        {
            atQQ = qq;
        }

        void delAtQQ(QQNumber qq)
        {
            atQQ = -1;
        }

        string getAtQQ()
        {
            if (atQQ != -1)
                return to_string(atQQ);

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