#include "lwc_builtins.h"
#include "lwc_typedefs.h"

namespace lwc {
	variable add(variable* vars, const variable& reg, const int& argc) {
		NumVar nv(vars[0]->get() + vars[1]->get());
		*reg = nv;
		return reg;
	}

	variable sub(variable* vars, const variable& reg, const int& argc) {
		*reg = NumVar(vars[0]->get() - vars[1]->get());
		return reg;
	}

	variable mult(variable* vars, const variable& reg, const int& argc) {
		*reg = NumVar(vars[0]->get() * vars[1]->get());
		return reg;
	}

	variable div(variable* vars, const variable& reg, const int& argc) {
		*reg = NumVar(vars[0]->get() / vars[1]->get());
		return reg;
	}

	variable while_loop(variable* vars, const variable& reg, const int& argc) {
		while (vars[0]->get()) {
			vars[1]->get();
		}
		return vars[0];
	}

	variable assign(variable* vars, const variable& reg, const int& argc) {
		*vars[0] = *vars[1];
		return vars[1];
	}

	variable refassign(variable* vars, const variable& reg, const int& argc) {
		vars[0] = vars[1];
		return vars[1];
	}

	variable is_lessthan(variable* vars, const variable& reg, const int& argc) {
		NumVar nv(vars[0]->get() < vars[1]->get());
		*reg = nv;
		return reg;
	}

	variable is_greaterthan(variable* vars, const variable& reg, const int& argc) {
		NumVar nv(vars[0]->get() > vars[1]->get());
		*reg = nv;
		return reg;
	}

	variable print(variable* vars, const variable& reg, const int& argc) {
		for (int i = 0; i < argc; i++) {
			std::cout << vars[i]->get() << " ";
		}
		std::cout << std::endl;
		return nullptr;
	}

	variable incrementby(lwc::variable* vars, const variable& reg, const int& argc) {
		NumVar nv(vars[0]->get() + vars[1]->get());
		*vars[0] = nv;
		return vars[0];
	}

	variable scast(lwc::variable* vars, const variable& reg, const int& argc) {
		std::shared_ptr<LASTVariable> templv = std::static_pointer_cast<LASTVariable>(vars[1]);
		vars[0] = templv;
		return vars[0];
	}
}