#ifndef ROBOT_INTERPRETER_H
#define ROBOT_INTERPRETER_H

#include <string>
#include <typeinfo>
#include "../stringutil.hpp"
#include "function.hpp"
#include "interpreters/js.hpp"
#include "interpreters/scheme.hpp"

using namespace std;
using namespace QQRobot;
using namespace Interpreters;

namespace QQRobot
{
    class Robot;

    class Interpreter : public Function
    {

    public:
        Interpreter() {}
        Interpreter(MessageSender *sender, Robot *robot) : Function(sender, robot) {}

        bool handleMessage(Message &fromMsg, Message &toMsg)
        {
            string content = fromMsg.getContent();
            string code = content.substr(content.find("eval:") + 5);
            string result;

            if (evalLanguage == "js")
            {
                try
                {
                    // 消息中的某些字符被编码，例如'[和']'被分别转换成了&#91;和&#93;，因此在这里先做解码
                    // 检查是否恶意代码
                    if (isBadCode(code))
                    {
                        if(typeid(fromMsg) == typeid(GroupMessage))
                            ((GroupMessage&)toMsg).setAtQQ(fromMsg.from);
                        (robot->blacklist).addQQ(fromMsg.from);
                        toMsg.setContent("你发的是恶意代码，你已经被关进小黑屋了！");
                        sender->sendMessage(toMsg);
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
            else if (evalLanguage == "scheme")
            {
                result = scheme.eval(code);
            }

            toMsg.setContent(result);
            sender->sendMessage(toMsg);
            return true;
        }
    private:
        string evalLanguage = "js";
        JS js;
        Scheme scheme;

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

        string toCode(string str)
        {
            str = stringutil::replace_all(str, "&#91;", "[");
            str = stringutil::replace_all(str, "&#93;", "]");
            str = stringutil::replace_all(str, "&amp;", "&");
            return str;
        }
    };
}

#endif