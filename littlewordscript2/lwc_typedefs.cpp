#include "lwc_typedefs.h"

long lwc::line_var::get() {
	eval.single_eval(line);
}