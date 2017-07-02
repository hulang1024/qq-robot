#include "group_message.hpp"
#include "message.hpp"
#include "message_sender.hpp"
#include "osu_query/osu_query.hpp"
#include "interpreters/js.hpp"
#include "interpreters/scheme.hpp"
#include "blacklist.hpp"
#include "stringutil.hpp"
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

        CQ_EVENT_RET onPrivateMessage(Message fromMsg)
        {
            string fromContent = fromMsg.getContent();

            Message toMsg;

            // 如果消息不是来自主人，就把该消息转发给主人
            if (fromMsg.from != masterQQ)
            {
                toMsg.to = masterQQ;
                toMsg.setContent("QQ" + fromMsg.from + "消息: " + fromContent);
                sender.sendPrivateMessage(toMsg);
                return EVENT_BLOCK;
            }

            // 执行群组消息代发命令，语法:!sendtogroup 目标群号 [某人QQ号] 暂不支持空格的消息
            if (fromContent.find("!sendtogroup") == 0)
            {
                // 只有主人有此命令权限
                if (fromMsg.from != masterQQ)
                    return EVENT_IGNORE;

                vector<string> strs = stringutil::split(fromContent, " ");
                GroupMessage toGpMsg;
                toGpMsg.to = strs[1];
                if (strs.size() > 3)
                {
                    toGpMsg.setAtQQ(strs[2]);
                    toGpMsg.setContent(strs[3]);
                }
                else
                {
                    toGpMsg.setContent(strs[2]);
                }
                sender.sendGroupMessage(toGpMsg);
            }
            // 执行私聊消息代发命令，语法:!send 某人QQ号 暂不支持空格的消息
            else if (fromContent.find("!send") == 0)
            {
                // 只有主人有此命令权限
                if (fromMsg.from != masterQQ)
                    return EVENT_IGNORE;

                vector<string> strs = stringutil::split(fromContent, " ");
                GroupMessage toMsg;
                toMsg.to = strs[1];
                toMsg.setContent(strs[2]);
                sender.sendPrivateMessage(toMsg);
            }

            return EVENT_IGNORE;
        }

        CQ_EVENT_RET onGroupMessage(GroupMessage fromMsg)
        {
            string fromContent = fromMsg.getContent();

            GroupMessage toMsg;
            toMsg.type = fromMsg.type;

            // 回复
            toMsg.to = fromMsg.groupQQ;
            int index;

            // 自己被AT
            bool atMe = fromMsg.getAtQQ() == this->qq;
            if (atMe)
            {
                //也AT对方
                toMsg.setAtQQ(fromMsg.from);

                if ((index = fromContent.find("功能")) != string::npos)
                {
                    toMsg.setContent(functionInfo());
                    sender.sendGroupMessage(toMsg);
                    return EVENT_BLOCK;
                }
            }

            if (fromContent.find("!stat") == 0)
            {
                //查询
                //wstring result = OsuQuery::query(fromContent);
                //toMsg.setContent(result);
                toMsg.setContent("暂未开发");
                sender.sendGroupMessage(toMsg);
                return EVENT_BLOCK;
            }
            else if((index = fromContent.find("eval:")) != string::npos)
            {
                if (blacklist.exist(fromMsg.from) || blacklist.exist("all"))
                {
                    toMsg.setAtQQ(fromMsg.from);
                    toMsg.setContent("你已经被关进小黑屋了:C");
                    sender.sendGroupMessage(toMsg);
                    return EVENT_BLOCK;
                }

                string code = fromContent.substr(index + 5);
                string result;

                if (defaultEvalLanguage == "js")
                {
                    try
                    {
                        // 消息中的某些字符被编码，例如'[和']'被分别转换成了&#91;和&#93;，因此在这里先做解码
                        // 检查是否恶意代码
                        if (isBadCode(code)) {
                            toMsg.setAtQQ(fromMsg.from);
                            blacklist.addQQ(fromMsg.from);
                            toMsg.setContent("你发的是恶意代码，你已经被关进小黑屋了！");
                            sender.sendGroupMessage(toMsg);
                            return EVENT_BLOCK;
                        }
                        code = toCode(code);
                        // 转换到Utf8
                        code = stringutil::string_To_UTF8(code);
                        // 执行代码，获取结果
                        result = js.evalForUTF8(code);
                        // 转换到string
                        result = stringutil::UTF8_To_string(result);
						result = result != "" ? result : " "; //解决不支持发送空
                        
                    }
                    catch (exception &e)
                    {
                        string info = "发生异常了:\n";
                        info += e.what();
                        result = info;
                    }
                }
                else if (defaultEvalLanguage == "scheme")
                {
                    result = scheme.eval(code);
                }

                toMsg.setContent(result);
                sender.sendGroupMessage(toMsg);
                return EVENT_BLOCK;
            }
            else if (fromContent.find("!blacklist") != string::npos)
            {
                if (fromMsg.from != masterQQ)
                {
                    toMsg.setAtQQ(fromMsg.from);
                    toMsg.setContent("无权限");
                    sender.sendGroupMessage(toMsg);
                    return EVENT_BLOCK;
                }
                vector<string> strs = stringutil::split(fromContent, " ");
                if (strs.size() < 2)
                    goto RET;
                string operatorStr = strs[1];

                if (operatorStr == "add" && strs.size() >= 3)
                {
                    string argStr = GroupMessage::tryGetQQFromAtContent(strs[2]);
                    blacklist.addQQ(argStr);
                    toMsg.setContent(argStr + " 成功关进小黑屋");
                }
                else if (operatorStr == "del" && strs.size() >= 3)
                {
                    string argStr = GroupMessage::tryGetQQFromAtContent(strs[2]);
                    blacklist.delQQ(argStr);
                    toMsg.setContent(argStr + " 成功从小黑屋释放");
                }
                else if (operatorStr == "list")
                {
                    toMsg.setContent(blacklist.empty() ? "黑名单空" : "黑名单:\n" + blacklist.toMutilLineStr(" "));
                }
                else if (operatorStr == "clear")
                {
                    blacklist.clear();
                    toMsg.setContent("成功清空黑名单");
                }
                sender.sendGroupMessage(toMsg);
                return EVENT_BLOCK;
            }
            else if ((index = fromContent.find("!set-eval-lang")) != string::npos)
            {
                defaultEvalLanguage = fromContent.substr(index + 14 + 1);
                toMsg.setContent("done");
                sender.sendGroupMessage(toMsg);
                return EVENT_BLOCK;
            }
            else if(atMe)
            {
                // echo
                toMsg.setContent(fromMsg.atContent());
                sender.sendGroupMessage(toMsg);
                return EVENT_BLOCK;
            }

            RET:
            return EVENT_IGNORE;
        }

        CQ_EVENT_RET onDiscussMessage(GroupMessage fromMsg)
        {
            fromMsg.type = 1;
            return onGroupMessage(fromMsg);
        }

    private:
        string qq;
        string defaultEvalLanguage = "js";
        JS js;
		Scheme scheme;
        BlackList blacklist;
        string masterQQ = "1013644379";

        string toCode(string str)
        {
            str = stringutil::replace_all(str, "&#91;", "[");
            str = stringutil::replace_all(str, "&#93;", "]");
            return str;
        }

        bool isBadCode(string code)
        {
            // 简单的恶意的
            vector<string> badCodes;
            badCodes.push_back("while(1)");
            badCodes.push_back("while(!0)");
            badCodes.push_back("while(true)");
            badCodes.push_back("while(!false)");
            badCodes.push_back("for(;;)");

            for (vector<string>::iterator it = badCodes.begin(); it != badCodes.end(); it++)
                if (code.find(*it) != string::npos)
                    return true;
            return false;
        }

        string functionInfo()
        {
            string info = "我的功能如下：\n";
            info += "  * 我可以执行JS程序，发送：eval: <code>`，例如:eval: 1+2。注意分号是英文的，分号后面可任意空白。\n";
            info += "  * 如果你@我，我也会@你。\n";
            info += " （注意：请不要写会导致我家（系统）异常的代码，当心小黑屋哦，在黑名单中意味着我会拒绝某些请求:D ）";
            return info;
        }


    };
}