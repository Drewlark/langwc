#include <vector>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <string>
#include <initializer_list>
#include "lwc_typedefs.h"
#ifndef H_LWC_LINE
#define H_LWC_LINE
using namespace std;

//typedef std::unordered_map<std::string, long> dataset;

class Line {
	int n;
public:
	static_varset vars;
	builtin_func func;
	bool request_last = false;
	bool loop = false;
	vector<Line> *linked_lines = nullptr;
	
	Line(std::vector<variable> _vars, builtin_func _func, bool _reqlast = false);

	Line(variable _var, builtin_func _func , bool _reqlast = false);

	~Line() { /*delete vars;*/ }

	int getN() {
		return n;
	}
};



typedef vector<Line> function;
typedef shared_ptr<vector<Line>> codeblock;
#endif // !H_LWC_LINE