#include <vector>
#include <iostream>
#include <iomanip>
#include "lwc_typedefs.h"
#ifndef LWC_BUILTINS
#define LWC_BUILTINS
using namespace std;

	
//ahhhhhhhhhh terrible memory leaks... will have to change this to use shared_ptrs or evaluate if returning rvalues is okay
namespace lwc {

	//variable mk_num_var() {};

	BaseVariable* add(lwc::varset &vars) {
		return new NumVar(vars[0]->get() + vars[1]->get());
	}

	BaseVariable* sub(lwc::varset& vars) {
		return new NumVar(vars[0]->get() - vars[1]->get());
	}

	BaseVariable* mult(lwc::varset& vars) {
		return new NumVar(vars[0]->get() * vars[1]->get());
	}

	BaseVariable* div(lwc::varset& vars) {
		return new NumVar(vars[0]->get() / vars[1]->get());
	}

	BaseVariable* while_loop(lwc::varset& vars) {
		while (vars[0]->get()) {
			vars[1]->get();
		}
		return vars[0];
	}

	variable assign(lwc::varset &vars) {
		*vars[0] = *vars[1];
		return vars[1];
	}

	variable is_lessthan(lwc::varset& vars) {
		return new NumVar(vars[0]->get() < vars[1]->get());
	}

	variable print(lwc::varset& vars) {
		cout << vars[0]->get() << endl;
		return nullptr;
	}

	/*long incrementby(lwc::varset &vars) {
		return (*vars[0] += *vars[1]);
	}

	long decrementby(lwc::varset &vars) {
		return (*vars[0] -= *vars[1]);
	}

	long print(lwc::varset &vars) {
		for (int i = 0; i < vars.n; i++) {
			cout << *vars[i] << " ";
		}
		cout << endl;
		return 1;
	}

	long is_equal(lwc::varset &vars) {
		return (*vars[0] == *vars[1]);
	}

	long is_greaterthan(lwc::varset &vars) {
		return (*vars[0] > *vars[1]);
	}

	

	
	


	long ret_val(lwc::varset &vars) {
		return vars[0]->get();
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