/*
Scheme解释器
author: hulang
*/
#include <string>
#include <stdlib.h>
#include "../../../libs/scheme/scm.h"

using namespace std;


namespace QQRobot
{
	namespace Interpreters
	{
		class Scheme
		{
		public:
			Scheme()
			{
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