#include "group_message.hpp"
#include "message_sender.hpp"
#include "osu_query/osu_query.hpp"
#include "interpreters/js/js.hpp"
#include <string.h>

using namespace Interpreters;

namespace QQRobot
{
    class Robot
    {
    public:
        MessageSender sender;

        Robot() { }
        Robot(MessageSender sender)
        {
            this->sender = sender;
        }

        void setQQ(string qq)
        {
            this->qq = qq;
        }

        CQ_EVENT_RET onGroupMessage(GroupMessage msg)
        {
            string fromContent = msg.getContent();

            // 回复
            msg.toGroupQQ = msg.fromGroupQQ;

            // 如果被AT，也AT对方
            if (msg.getAtQQ() == this->qq) {
                msg.setAtQQ(msg.fromQQ);
            }

            if (fromContent.find("!stat") == 0)
            {
                //查询
                string result = OsuQuery::query(fromContent);
                msg.setContent(result);
                sender.sendMessageToGroup(msg);
                return EVENT_BLOCK;
            }
            else if(fromContent.find("eval:") == 0)
            {
                string code = fromContent.substr(5);
                string result;
                if (defaultEvalLanguage == "js")
                {
                    result = js.evalForUTF8(code);
                }
                msg.setContent(result);
                sender.sendMessageToGroup(msg);
                return EVENT_BLOCK;
            }
            else if (fromContent.find("set!") == 0)
            {
                if (msg.fromQQ == 1013644379) {
                    msg.setContent("succeed");
                    sender.sendMessageToGroup(msg);
                    return EVENT_BLOCK;
                }
            }

            return EVENT_IGNORE;
        }

    private:
        string qq;
        string defaultEvalLanguage = "js";
        JS js;
	};
}