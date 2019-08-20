#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>
#include "lwc_RegisterStore.h"
#include "lwc_typedefs.h"
#ifndef LWC_BUILTINS
#define LWC_BUILTINS
using namespace std;

// #define make_num 

namespace lwc {

	//variable mk_num_var() {};

	variable add(variable* vars, const variable& reg, const int& argc) {
		NumVar nv(vars[0]->get() + vars[1]->get());
		*reg = nv;
		return reg;
	}

	variable sub(variable* vars, const variable &reg, const int& argc) {
		*reg = NumVar(vars[0]->get() - vars[1]->get());
		return reg;
	}

	variable mult(variable* vars, const variable &reg, const int& argc) {
		*reg = NumVar(vars[0]->get() * vars[1]->get());
		return reg;
	}

	variable div(variable* vars, const variable &reg, const int& argc) {
		*reg = NumVar(vars[0]->get() / vars[1]->get());
		return reg;
	}

	variable while_loop(variable* vars, const variable &reg, const int& argc) {
		while (vars[0]->get()) {
			vars[1]->get();
		}
		return vars[0];
	}

	variable assign(variable* vars, const variable &reg, const int& argc) {
		*vars[0] = *vars[1];
		return vars[1];
	}

	variable is_lessthan(variable* vars, const variable &reg, const int& argc) {
		NumVar nv(vars[0]->get() < vars[1]->get());
		*reg = nv;
		return reg;
	}

	variable print(variable* vars, const variable &reg, const int& argc) {
		cout << vars[0]->get() << endl;
		return vars[0];
	}

	variable incrementby(lwc::variable* vars, const variable& reg, const int& argc) {
		NumVar nv(vars[0]->get() + vars[1]->get());
		*vars[0] = nv;
		return vars[0];
	}

	/*static const std::unordered_map<lwc::builtin_func, bool> rval = {
		{}
	};


	


	/*
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