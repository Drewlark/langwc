#include <vector>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <string>
#include <initializer_list>

#ifndef H_LWC_LINE
#define H_LWC_LINE
using namespace std;

typedef shared_ptr<long> variable;
typedef vector<variable> varset;
using builtin_func = long(*)(const varset&);
//typedef std::unordered_map<std::string, long> dataset;

class Line {
public:
	std::vector<variable> vars;
	builtin_func func;
	bool request_last = false;
	bool loop = false;
	vector<Line> *linked_lines = nullptr;

	Line() { vars = vector<variable>();}
	Line(std::vector<variable> _vars);
	
	Line(std::vector<variable> _vars, builtin_func);
	
	Line(variable _var, builtin_func _func);

	Line(std::vector<variable> _vars, builtin_func _func, bool _reqlast);

	Line(variable _var, builtin_func _func , bool _reqlast);
};



typedef vector<Line> function;
typedef shared_ptr<vector<Line>> codeblock;
#endif // !H_LWC_LINE