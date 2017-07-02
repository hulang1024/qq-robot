#include <string>
#include <stdlib.h>

using namespace std;


#define scm_void ((ptr)0x3E)

typedef void * ptr;

typedef ptr(*scm_init_fn)();
typedef ptr(*scm_read_string_fn)(char *);
typedef char*(*scm_strings_fn)(ptr);
typedef ptr(*scm_string_fn)(char *);
typedef ptr(*scm_eval_fn)(char *);
typedef ptr(*scm_eval_exp_fn)(ptr);
typedef void(*scm_deinit_fn)();
typedef ptr(*scm_call0_fn)(ptr);
typedef ptr(*scm_call1_fn)(ptr, ptr);
typedef ptr(*scm_get_thread_context_fn)();

scm_init_fn scm_init;
scm_read_string_fn scm_read_string;
scm_strings_fn scm_strings;
scm_string_fn scm_string;
scm_eval_fn scm_eval;
scm_eval_exp_fn scm_eval_exp;
scm_deinit_fn scm_deinit;
scm_call0_fn scm_call0;
scm_call1_fn scm_call1;
scm_get_thread_context_fn scm_get_thread_context;

namespace QQRobot
{
	namespace Interpreters
	{
		class Scheme
		{
		public:
			Scheme()
			{
				HINSTANCE hdll;
				hdll = LoadLibrary(L"libscm.dll");
				if (hdll == NULL) {
					errormsg = "hdll is null";
					return;
				}
				scm_init = (scm_init_fn)GetProcAddress(hdll, "scm_init");
				scm_read_string = (scm_read_string_fn)GetProcAddress(hdll, "scm_read_string");
				scm_string = (scm_string_fn)GetProcAddress(hdll, "scm_string");
				scm_strings = (scm_strings_fn)GetProcAddress(hdll, "scm_strings");
				scm_eval = (scm_eval_fn)GetProcAddress(hdll, "scm_eval");
				scm_eval_exp = (scm_eval_exp_fn)GetProcAddress(hdll, "scm_eval_exp");
				scm_deinit = (scm_deinit_fn)GetProcAddress(hdll, "scm_deinit");
				scm_call0 = (scm_call0_fn)GetProcAddress(hdll, "scm_call0");
				scm_call1 = (scm_call1_fn)GetProcAddress(hdll, "scm_call1");
				scm_get_thread_context = (scm_get_thread_context_fn)GetProcAddress(hdll, "scm_get_thread_context");

				scm_init();

			}

			~Scheme()
			{
				scm_deinit();
			}

			string eval(string str)
			{
				

				if (errormsg != NULL)
					return errormsg;
				if (scm_read_string == NULL)
					return "scm_read_string is null";
				ptr sexp = scm_read_string( (char*)str.c_str() );
				if (sexp == NULL)
					return "sexp is null";
				ptr sval = scm_eval_exp(sexp);
				if (sval == NULL)
					return "sval is null";
				string cstr = scm_strings(sval);
				return cstr;
			}
		private:
			char* errormsg = NULL;
		};
	}
}