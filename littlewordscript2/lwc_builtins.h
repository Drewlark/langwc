#include <vector>
#include <iostream>
#include <iomanip>
#include "lwc_typedefs.h"
#ifndef LWC_BUILTINS
#define LWC_BUILTINS
using namespace std;



namespace lwc {

	long add(lwc::static_varset &vars) {
		return *vars[0] + *vars[1];
	}

	long sub(lwc::static_varset &vars) {
		return *vars[0] - *vars[1];
	}

	long mult(lwc::static_varset& vars) {
		return *vars[0] * *vars[1];
	}

	long div(lwc::static_varset& vars) {
		return *vars[0] / *vars[1];
	}

	long assign(lwc::static_varset &vars) {
		*vars[0] = *vars[1];
		return *vars[1];
	}

	long incrementby(lwc::static_varset &vars) {
		return (*vars[0] += *vars[1]);
	}

	long decrementby(lwc::static_varset &vars) {
		return (*vars[0] -= *vars[1]);
	}

	long print(lwc::static_varset &vars) {
		for (int i = 0; i < vars.n; i++) {
			cout << *vars[i] << " ";
		}
		cout << endl;
		return 1;
	}

	long is_equal(lwc::static_varset &vars) {
		return (*vars[0] == *vars[1]);
	}

	long is_greaterthan(lwc::static_varset &vars) {
		return (*vars[0] > *vars[1]);
	}

	/*long is_lessthan(lwc::static_varset &vars) {
		variable refvar = vars[0];
		for (long i = 1; i < vars.size(); i++) {
			if (*refvar >= *vars[i]) {
				return 0;
			}
			refvar = vars[i];
		}
		return 1;
	}*/

	long is_lessthan(lwc::static_varset &vars) {
		return (*vars[0] < *vars[1]);
	}
	


	long ret_val(lwc::static_varset &vars) {
		return *vars[0];
	}

	/*long func_run(lwc::static_varset &vars) {
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