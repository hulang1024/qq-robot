#include "group_message.hpp"
#include "message_sender.hpp"
#include "osu_query/osu_query.hpp"
#include "interpreters/js/js.hpp"
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

        CQ_EVENT_RET onGroupMessage(GroupMessage msg)
        {
            CQ_EVENT_RET returnResult = EVENT_IGNORE;

            string fromContent = msg.getContent();

            // 回复
            msg.toGroupQQ = msg.fromGroupQQ;
            int index;

            // 自己被AT
            if (msg.getAtQQ() == this->qq)
            {
                //也AT对方
                msg.setAtQQ(msg.fromQQ);

                if ((index = fromContent.find("功能")) != string::npos)
                {
                    msg.setContent(functionInfo());
                }
                else
                {
                    // echo
                    msg.setContent(msg.atContent());
                }

                returnResult = EVENT_BLOCK;
                
            }

            if (fromContent.find("!stat") == 0)
            {
                //查询
                //wstring result = OsuQuery::query(fromContent);
                //msg.setContent(result);
                msg.setContent("暂未开发");
                returnResult = EVENT_BLOCK;
            }
            else if((index = fromContent.find("eval:")) != string::npos)
            {
                if (blacklist.exist(to_string(msg.fromQQ)))
                {

                    msg.setAtQQ(msg.fromQQ);
                    msg.setContent("你已经被关进小黑屋了:C");
                    returnResult = EVENT_BLOCK;
                    goto RETMSG;
                }

                if (defaultEvalLanguage == "js")
                {
                    try
                    {
                        string code = fromContent.substr(index + 5);
                        // 消息中的某些字符被编码，例如'[和']'被分别转换成了&#91;和&#93;，因此在这里先做解码
                        code = toCode(code);
						// 转换到Utf8
                        code = stringutil::string_To_UTF8(code);
						// 执行代码，获取结果
                        string result = js.evalForUTF8(code);
						// 转换到string
                        result = stringutil::UTF8_To_string(result);
                        msg.setContent(result != "" ? result : " "); //解决不支持发送空
                    }
                    catch (exception &e)
                    {
                        string info = "发生异常了:\n";
                        info += e.what();
                        msg.setContent(info);
                    }
                    returnResult = EVENT_BLOCK;
                }
            }
            else if (fromContent.find("!blacklist") != string::npos)
            {
                if (msg.fromQQ != 1013644379)
                {
                    msg.setAtQQ(msg.fromQQ);
                    msg.setContent("无权限");
                    returnResult = EVENT_BLOCK;
                    goto RETMSG;
                }
                vector<string> strs = stringutil::split(fromContent, " ");
                if (strs.size() < 2)
                    goto RET;
                string operatorStr = strs[1];

                if (operatorStr == "add" && strs.size() >= 3)
                {
                    string argStr = GroupMessage::tryGetQQFromAtContent(strs[2]);
                    blacklist.addQQ(argStr);
                    msg.setContent(argStr + " 成功关进小黑屋");
                }
                else if (operatorStr == "del" && strs.size() >= 3)
                {
                    string argStr = GroupMessage::tryGetQQFromAtContent(strs[2]);
                    blacklist.delQQ(argStr);
                    msg.setContent(argStr + " 成功从小黑屋释放");
                }
                else if (operatorStr == "list")
                {
                    msg.setContent(blacklist.empty() ? "黑名单空" : "黑名单:\n" + blacklist.toMutilLineStr("  "));
                }
                else if (operatorStr == "clear")
                {
                    blacklist.clear();
                    msg.setContent("成功清空黑名单");
                }
                returnResult = EVENT_BLOCK;
            }

            RETMSG:
            if(returnResult == EVENT_BLOCK)
                sender.sendMessageToGroup(msg);

            RET:
            return returnResult;
        }

    private:
        string qq;
        string defaultEvalLanguage = "js";
        JS js;
        BlackList blacklist;

        string toCode(string str)
        {
            str = stringutil::replace_all(str, "&#91;", "[");
            str = stringutil::replace_all(str, "&#93;", "]");
            return str;
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