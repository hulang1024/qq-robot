/*
机器人，监听消息事件
author: hulang
*/
#include "robot.h"
#include "functions/function.hpp"
#include "functions/osu_query/osu_query.hpp"
#include "functions/blacklist.hpp"
#include "functions/manual.hpp"
#include "functions/weather_forecast.hpp"
#include "functions/interpreter.hpp"
#include "functions/games/bullsandcows.hpp"

using namespace QQRobot;

Robot::Robot()
{
    //if(initance == NULL)
    nickname = "超蓝猫";
    qq = "3381775672";
    masterQQ = "1013644379";

    solts["man"] = new Manual();
    solts["osuQuery"] = new OsuQuery();
    solts["black"] = new BlackList();
    solts["weatherForecast"] = new WeatherForecast();
    solts["interpreter"] = new Interpreter();
}

Robot::~Robot()
{
    for (map<string, void*>::iterator iter = solts.begin(); iter != solts.end(); iter++)
        delete iter->second;
}

static Robot *robotInstance = NULL;

Robot* Robot::initance()
{
    if (robotInstance == NULL) {
        robotInstance = new Robot();
    }
    return robotInstance;
}

CQ_EVENT_RET Robot::onPrivateMessage(PrivateMessage &fromMsg)
{
    fromMsg.setContent(stringutil::trim(fromMsg.getContent()));
    string fromContent = fromMsg.getContent();

    PrivateMessage toMsg;
    toMsg.to = fromMsg.from;

    // 如果消息不是来自主人，就把该消息转发给主人
    if (fromMsg.from != masterQQ)
    {
        toMsg.to = masterQQ;
        toMsg.setContent("QQ" + fromMsg.from + "消息: " + fromContent);
        sender->sendPrivateMessage(toMsg);
        return EVENT_BLOCK;
    }

    Function *func = NULL;

    if (fromContent.find("sendtogroup") == 0)
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
        sender->sendGroupMessage(toGpMsg);
    }
    else if (fromContent.find("send") == 0)
    {
        // 只有主人有此命令权限
        if (fromMsg.from != masterQQ)
            return EVENT_IGNORE;

        vector<string> strs = stringutil::split(fromContent, " ");
        PrivateMessage toMsg;
        toMsg.to = strs[1];
        toMsg.setContent(strs[2]);
        sender->sendPrivateMessage(toMsg);
    }
    else if (fromContent.find("black") != string::npos)
        func = (Function*)solts["black"];

    if (func != NULL)
    {
        func->robot = this;
        bool handleBlock = func->handleMessage(fromMsg, toMsg);
        if (handleBlock)
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQ_EVENT_RET Robot::onGroupMessage(GroupMessage &fromMsg)
{
    fromMsg.setContent(stringutil::trim(fromMsg.getContent()));
    string fromContent = fromMsg.getContent();

    GroupMessage toMsg;
    toMsg.type = fromMsg.type;

    // 回复
    toMsg.to = fromMsg.groupQQ;

    // 自己被AT
    bool atMe = fromMsg.getAtQQ() == this->qq;
    if (atMe)
    {
        //也AT对方
        toMsg.setAtQQ(fromMsg.from);
    }

    if (fromContent.find("function-list") != string::npos || fromContent.find("@功能") != string::npos)
    {
        toMsg.setContent("\n" + Function::functionInfo());
        sender->sendGroupMessage(toMsg);
        return EVENT_BLOCK;
    }

    else if (fromContent.find("about") != string::npos || fromContent.find("@关于") != string::npos)
    {
        string aboutinfo = "\n关于:\n";
        aboutinfo += "工程: https://github.com/hlpp/qq-robot/\n";
        aboutinfo += "开发者们: problue(hlpp) .\n";
        aboutinfo += code_msg_face(49) + "欢迎加入.\n";
        toMsg.setContent(aboutinfo);
        sender->sendGroupMessage(toMsg);
        return EVENT_BLOCK;
    }

    Function *func = NULL;

    if (fromContent.find("!man") != string::npos)
        func = (Function*)solts["man"];
    else if (fromContent.find("!stat") != string::npos)
        func = (Function*)solts["osuQuery"];
    else if (fromContent.find("eval:") != string::npos)
        func = (Function*)solts["interpreter"];
    else if (fromContent.find("black") != string::npos)
        func = (Function*)solts["blacklist"];
    else if (fromContent.find("天气") != string::npos)
        func = (Function*)solts["weatherForecast"];
    else if (fromContent.find("AB") != string::npos) {
        string soltKey = "groupQQ_" + fromMsg.groupQQ + "_AB";
        func = (Function*)solts[soltKey];
        if (func == NULL) {
            func = new BullsAndCows();
            solts[soltKey] = func;
        }
    }
    else if (atMe)
    {
        // echo
        string atContent = fromMsg.atContent();
        toMsg.setContent(atContent.length() > 0 ? atContent : "在下" + nickname + ", 有何贵干？请发送“@功能”");
        sender->sendGroupMessage(toMsg);
        return EVENT_BLOCK;
    }

    if (func != NULL)
    {
        BlackList *black = (BlackList*)solts["black"];
        if (black->exist(fromMsg.from) || black->exist("all"))
        {
            toMsg.setAtQQ(fromMsg.from);
            toMsg.setContent("你已经被关进小黑屋了:C");
            sender->sendGroupMessage(toMsg);
            return EVENT_BLOCK;
        }

        func->robot = this;
        Function::handle_message_code retCode;
        try
        {
            retCode = func->handleMessage(fromMsg, toMsg);
        }
        catch (exception &e) {

        }
        switch (retCode)
        {
        case Function::handle_message_code::block:
            return EVENT_BLOCK;
        case Function::handle_message_code::syntax_error:
            toMsg.setContent(((Manual*)solts["man"])->getInfo("man"));
            sender->sendMessage(toMsg);
            sender->sendGroupMessage(toMsg);
        }
    }

    return EVENT_IGNORE;
}

CQ_EVENT_RET Robot::onDiscussMessage(GroupMessage &fromMsg)
{
    fromMsg.type = 1;
    return onGroupMessage(fromMsg);
}
