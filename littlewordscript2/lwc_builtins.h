#include <vector>
#include <iostream>
#include <iomanip>
#ifndef LWC_BUILTINS
#define LWC_BUILTINS
using namespace std;

namespace lwc {

	long add(const varset &vars) {
		return *vars[0] + *vars[1];
	}

	long sub(const varset &vars) {
		return *vars[0] - *vars[1];
	}

	long assign(const varset &vars) {
		*vars[0] = *vars[1];
		return 1;
	}

	long incrementby(const varset &vars) {
		return (*vars[0] += *vars[1]);
	}

	long decrementby(const varset &vars) {
		return (*vars[0] -= *vars[1]);
	}

	long print(const varset &vars) {
		cout << ">:  ";
		for (variable var : vars) {
			cout << *var << " ";
		}
		cout << endl;
		return 0;
	}

	long is_equal(const varset &vars) {
		const variable refvar = vars[0];
		for (long i = 1; i < vars.size(); i++) {
			if (*refvar != *vars[i]) {
				return 0;
			}
		}
		return 1;
	}

	long is_greaterthan(const varset &vars) {
		variable refvar = vars[0];
		for (long i = 1; i < vars.size(); i++) {
			if (*refvar <= *vars[i]) {
				return 0;
			}
			refvar = vars[i];
		}
		return 1;
	}

	/*long is_lessthan(const varset &vars) {
		variable refvar = vars[0];
		for (long i = 1; i < vars.size(); i++) {
			if (*refvar >= *vars[i]) {
				return 0;
			}
			refvar = vars[i];
		}
		return 1;
	}*/

	long is_lessthan(const varset &vars) {
		return (*vars[0] < *vars[1]);
	}

	const long& evaluate(const vector<Line> &linevec)
	{
		static long last_eval = 0;
		for (const Line &ln : linevec) {
			if (!ln.request_last)
				last_eval = ln.func(ln.vars);
			else
			{
				vector<variable> newvars = ln.vars;
				//newvars.resize(ln.vars.size());
				newvars.push_back(variable(new long(last_eval)));
				last_eval = ln.func(newvars);
			}
			if (ln.linked_lines != nullptr && ln.linked_lines->size() > 0 && last_eval) {
				
				if (ln.loop) {
					while (ln.func(ln.vars)) {
						evaluate((*ln.linked_lines));
					}
				}
				else {
					evaluate((*ln.linked_lines));
				}
			}
		}
		return last_eval;
	}


	long ret_val(const varset &vars) {
		return *vars[0];
	}

	/*long func_run(const varset &vars) {
		vector<Line> *func = (vector<Line>*)(*vars[0]);
		if (vars.size() > 1) {
			for (auto i = vars.begin() + 1; i < vars.end(); i++) {
				varset as_vars;
				variable n = make_shared<long>(*i);
				as_vars.push_back(n);
				func->insert(func->begin(), Line(vars));
			}
		}
		//func->insert(func->begin(), )
	}*/
	
	

}
#endif //!LWC_BUILTINS