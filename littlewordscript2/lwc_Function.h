#include "lwc_typedefs.h"
#include "lwc_Line.h"
#include <vector>
#ifndef LWC_FUNCTION
#define LWC_FUNCTION

struct Function {
	varset params;
	codeblock func_code;
	Function(varset _params, codeblock _func_code) : params(_params), func_code(_func_code) {}
};

vector<Function> master_set;

#endif