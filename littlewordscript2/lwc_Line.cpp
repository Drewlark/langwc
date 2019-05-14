#include "lwc_Line.h"
#include "lwc_typedefs.h"
Line::Line(std::vector<variable> _vars, builtin_func _func, bool _reqlast) {
	n = _vars.size() + _reqlast;
	static_varset test(_vars, n);
	vars = test;
	func = _func;
	request_last = _reqlast;
};

Line::Line(variable _var, builtin_func _func, bool _reqlast) {
	n = 1 + _reqlast;

	vars = static_varset(new variable(_var), n);
	func = _func;
	request_last = _reqlast;
};
