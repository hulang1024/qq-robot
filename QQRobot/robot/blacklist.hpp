#include <string>
#include <map>

using namespace std;
using namespace QQRobot;

namespace QQRobot
{
    class BlackList
    {
    public:

        void addQQ(string qq)
        {
            mmap[qq] = true;
        }

        void delQQ(string qq)
        {
            mmap.erase(qq);
        }

        void clear()
        {
            mmap.clear();
        }
        
        bool exist(string qq)
        {
            return mmap.count(qq) > 0;
        }
        
        bool empty()
        {
            return mmap.size() == 0;
        }

        string toMutilLineStr(char* indent)
        {
            string str;
            map<string, bool>::iterator iter;
            for (iter = mmap.begin(); iter != mmap.end(); iter++)
                str += indent + iter->first + "\n";
            return str;
        }

    private:
        map<string, bool> mmap;
    };
}