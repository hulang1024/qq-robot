#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
#include <vector>
#include <wchar.h>
#include <windows.h>

using namespace std;

namespace stringutil
{
    string& replace_all(string& str, const string& old_value, const string& new_value);

    string UTF8_To_string(const string & str);

    string string_To_UTF8(const string & str);

    vector<string> split(string str, string pattern);
}

#endif