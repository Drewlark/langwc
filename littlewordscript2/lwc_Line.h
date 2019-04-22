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

typedef shared_ptr<int> variable;
typedef vector<variable> varset;
using builtin_func = int(*)(const varset&);
typedef std::unordered_map<std::string, int> dataset;

class Line {
public:
	std::vector<variable> vars;
	builtin_func func;
	bool request_last = false;
	Line() { vars = vector<variable>();}
	Line(std::vector<variable> _vars);
	
	Line(std::vector<variable> _vars, builtin_func);
	
	Line(variable _var, builtin_func _func);

	Line(std::vector<variable> _vars, builtin_func _func, bool _reqlast);

	Line(variable _var, builtin_func _func , bool _reqlast);
};



typedef vector<Line> function;

#endif // !H_LWC_LINE