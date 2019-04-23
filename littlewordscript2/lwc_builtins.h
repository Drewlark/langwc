#include <vector>
#include <iostream>
#include <iomanip>
#ifndef LWC_BUILTINS
#define LWC_BUILTINS
using namespace std;

namespace lwc {
	int add(const varset &vars) {
		return *vars[0] + *vars[1];
	}

	int sub(const varset &vars) {
		return *vars[0] - *vars[1];
	}

	int assign(const varset &vars) {
		*vars[0] = *vars[1];
		return 1;
	}

	int print(const varset &vars) {
		cout << ">:  ";
		for (variable var : vars) {
			cout << *var << " ";
		}
		cout << endl;
		return 0;
	}

	int evaluate(vector<Line> linevec)
	{
		static int last_eval = 0;
		for (Line ln : linevec) {
			if (!ln.request_last)
				last_eval = ln.func(ln.vars);
			else
			{
				vector<variable> newvars = ln.vars;
				newvars.push_back(variable(new int(last_eval)));
				last_eval = ln.func(newvars);
			}
		}
		return 0;
	}

	int ret_val(const varset &vars) {
		return *vars[0];
	}

	int block_run(const varset &vars) {
		vector<Line> *func = (vector<Line>*)(*vars[0]);
		return evaluate(*func);
	}

	/*int func_run(const varset &vars) {
		vector<Line> *func = (vector<Line>*)(*vars[0]);
		if (vars.size() > 1) {
			for (auto i = vars.begin() + 1; i < vars.end(); i++) {
				varset as_vars;
				variable n = make_shared<int>(*i);
				as_vars.push_back(n);
				func->insert(func->begin(), Line(vars));
			}
		}
		//func->insert(func->begin(), )
	}*/
	
	

}
#endif //!LWC_BUILTINS