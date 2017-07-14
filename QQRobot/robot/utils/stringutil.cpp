#include "stringutil.h"
namespace stringutil
{
    string trim(string s)
    {
        size_t len = s.length();
        size_t si, ei;
        size_t i;
        for (i = 0; i < len && isspace(s[i]);) i++;
        si = i;
        for (i = len - 1; i >= 0 && isspace(s[i]);) i--;
        ei = i;

        return s.substr(si, ei + 1 - si);
    }

    string& replace_all(string& str, const string& old_value, const string& new_value)
    {
        while (true) {
            string::size_type   pos(0);
            if ((pos = str.find(old_value)) != string::npos)
                str.replace(pos, old_value.length(), new_value);
            else
                break;
        }
        return str;
    }

    string UTF8_To_string(const string & str)
    {
        int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

        wchar_t * pwBuf = new wchar_t[nwLen + 1];
        memset(pwBuf, 0, nwLen * 2 + 2);

        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

        int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

        char * pBuf = new char[nLen + 1];
        memset(pBuf, 0, nLen + 1);

        WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

        std::string retStr = pBuf;

        delete[]pBuf;
        delete[]pwBuf;

        pBuf = NULL;
        pwBuf = NULL;

        return retStr;
    }

    string string_To_UTF8(const string & str)
    {
        int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

        wchar_t * pwBuf = new wchar_t[nwLen + 1];
        ZeroMemory(pwBuf, nwLen * 2 + 2);

        ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

        int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

        char * pBuf = new char[nLen + 1];
        ZeroMemory(pBuf, nLen + 1);

        ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

        std::string retStr(pBuf);

        delete[]pwBuf;
        delete[]pBuf;

        pwBuf = NULL;
        pBuf = NULL;

        return retStr;
    }

    vector<string> split(string str, string pattern)
    {
        size_t pos;
        std::vector<std::string> result;
        str += pattern;
        size_t size = str.size();

        for (size_t i = 0; i < size; i++)
        {
            pos = str.find(pattern, i);
            if (pos < size)
            {
                std::string s = str.substr(i, pos - i);
                result.push_back(s);
                i = pos + pattern.size() - 1;
            }
        }
        return result;
    }
}