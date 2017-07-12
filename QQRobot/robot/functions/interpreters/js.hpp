/*
JS解释器
author: hulang
*/
#include <string>
#include "../../../libs/v8/include/v8.h"

using namespace std;
using namespace v8;

namespace QQRobot
{
    namespace Interpreters
    {
        class JS
        {
        public:
            JS()
            {

            }

            // code: utf-8
            string evalForUTF8(string code)
            {
                isolate = Isolate::New();
                isolate->Enter();

                HandleScope handle_scope(isolate);

                // 创建一个新上下文
                Handle<Context> context = Context::New(isolate);

                Context::Scope context_scope(context);

                Handle<String> source = String::NewFromUtf8(isolate, code.c_str());

                TryCatch trycatch;

                // 编译源码
                Handle<Script> script = Script::Compile(source);

                // 运行
                Handle<Value> result = script->Run();
                if (result.IsEmpty())
                {
                    Local<Value> ex = trycatch.Exception();
                    String::Utf8Value exUtf8(ex);
                    return string(*exUtf8);
                }

                String::Utf8Value utf8(result);
                return string(*utf8);
            }
        private:
            Isolate* isolate;
        };
    }
}