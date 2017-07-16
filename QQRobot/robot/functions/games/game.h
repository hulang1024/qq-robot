/*
游戏
author: hulang
*/
#ifndef ROBOT_GAME_H
#define ROBOT_GAME_H

#include "robot/functions/function.hpp"
#include "../../robot.h"

using namespace QQRobot;

namespace QQRobot {
    class Game : public Function {
    public:
        enum GameState {
            unstarted = 0,
            started,
            playing,
            ended
        };
    protected:
        GameState gameState = unstarted;
    };
}

#endif