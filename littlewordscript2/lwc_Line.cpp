#include "lwc_Line.h"
#include "lwc_typedefs.h"
Line::Line(std::vector<variable> _vars, builtin_func _func, bool _reqlast) {
	vars = _vars;
	func = _func;
	request_last = _reqlast;
};

Line::Line(variable _var, builtin_func _func, bool _reqlast) {
	vars = std::vector<variable>({ _var });
	func = _func;
	request_last = _reqlast;
};
