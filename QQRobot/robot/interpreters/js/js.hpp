#include <string>
#include "v8/include/v8.h"

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
                // Get the default Isolate created at startup.
                isolate = Isolate::New();
                isolate->Enter();
                // Create a stack-allocated handle scope.
                HandleScope handle_scope(isolate);

                // Create a new context.
                Handle<Context> context = Context::New(isolate);

                // Enter the context for compiling and running the hello world script.
                Context::Scope context_scope(context);

                // Create a string containing the JavaScript source code.
                Handle<String> source = String::NewFromUtf8(isolate, code.c_str());

                TryCatch trycatch;
                // Compile the source code.
                Handle<Script> script = Script::Compile(source);

                // Run the script to get the result.
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