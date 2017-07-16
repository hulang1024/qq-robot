/*
Bulls And Cows标准规则猜数字
author: hulang
*/
#include <string>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "../../functions/function.hpp"
#include "../../robot.h"

using namespace std;
using namespace QQRobot;

namespace QQRobot {
    class BullsAndCows : public Game {
    public:
        handle_message_code handleMessage(Message &fromMsg, Message &toMsg) {
            string output;
            vector<string> args = stringutil::split(fromMsg.getContent(), " ");

            LOOP:
            switch (gameState) {
            case unstarted:
            case ended:
                genRandQuestion();
                times = 0;
                output = "我已经想出了一个无重复的四位数字，请开始猜吧~";
                gameState = playing;
                break;
            case started:
            case playing:
                if (args.size() > 1) {
                    if (args[1] == "restart") {
                        gameState = unstarted;
                        goto LOOP;
                    }
                    else if (args[1] == "end") {
                        gameState = ended;
                        output = "本次游戏已结束";
                    }
                    else if (args[1] == "show") {
                        output = "偷偷告诉你，答案是 " + string(answer);
                    }
                    else {
                        string input = args[1];
                        if (input.length() != ANSWER_N) {
                            output = "答案位数不对";
                            break;
                        }

                        times++;
                        output = feedback(input);
                        if (output == "4A0B") {
                            output += "\n" + code_msg_at(fromMsg.from)
                                   + "\n恭喜你，猜对了！用了" + to_string(times) + "次。";
                            gameState = ended;
                        }
                    }
                }
                else {
                    output = "缺少答案";
                    break;
                }
                break;
            }

            toMsg.setContent(output);
            robot->sender->sendMessage(toMsg);
            return handle_message_code::block;
        }

    private:
        static const size_t ANSWER_N = 4;
        char answer[ANSWER_N + 1] = {0};
        int times;

        void genRandQuestion() {
            srand(time(NULL));

            const size_t N = 10;
            char seedArray[] = "0123456789";
            for (size_t i = 0; i < ANSWER_N; i++) {
                size_t r = rand() % (N - i);
                answer[i] = seedArray[r];
                seedArray[r] = seedArray[N - i - 1];
            }

        }

        string feedback(string input) {
            int A = 0; //位置正确的数的个数
            int B = 0; //数字正确而位置不对的数的个数

            for (size_t ai = 0; ai < ANSWER_N; ai++) {
                if (answer[ai] == input[ai])
                    A++;
                else {
                    for (size_t i = 0; i < ANSWER_N; i++)
                        if (ai != i && answer[i] == input[ai])
                            B++;
                }
            }

            return to_string(A) + "A" + to_string(B) + "B";
        }
    };
}