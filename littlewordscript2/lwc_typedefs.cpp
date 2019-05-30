#include "lwc_typedefs.h"

long lwc::line_var::get() {
	return eval.single_eval(line);
}