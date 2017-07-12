#include <map>

using namespace std;

namespace QQRobot
{
    namespace OsuQuery
    {
        //TODO:
        class Request
        {
        public:
            Request(string url)
            {
            }
            void addParam(string name, string value)
            {
                params[name] = value;
            }

        private:
            map<string, string> params;
        };
    }
}