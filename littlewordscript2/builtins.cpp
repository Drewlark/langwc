#include "lwc_builtins.h"
#include "lwc_typedefs.h"
#include "lwc_err_codes.h"

namespace lwc {
	variable& add(variable** vars, variable& reg, const int& argc) {
		NumVar nv((*vars[0])->get() + (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	variable& sub(variable** vars, variable& reg, const int& argc) {
		NumVar nv((*vars[0])->get() - (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	variable& mult(variable** vars, variable& reg, const int& argc) {
		NumVar nv((*vars[0])->get() * (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	variable& div(variable** vars, variable& reg, const int& argc) {
		NumVar nv((*vars[0])->get() / (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	variable& while_loop(variable** vars, variable& reg, const int& argc) {
		while ((*vars[0])->get()) {
			(*vars[1])->get();
		}
		return *vars[0];
	}

	variable& assign(variable** vars, variable& reg, const int& argc) {
		**vars[0] = **vars[1];
		return *vars[1];
	}

	variable& refassign(variable** vars, variable& reg, const int& argc) {
		*vars[0] = *vars[1];
		return *vars[1];
	}

	variable& is_lessthan(variable** vars, variable& reg, const int& argc) {
		NumVar nv((*vars[0])->get() < (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	variable& is_greaterthan(variable** vars, variable& reg, const int& argc) {
		NumVar nv((*vars[0])->get() > (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	variable& print(variable** vars, variable& reg, const int& argc) {
		for (int i = 0; i < argc; i++) {
			std::cout << (*vars[i])->get() << " ";
		}
		std::cout << std::endl;
		return reg;
	}

	variable& incrementby(lwc::variable** vars, variable& reg, const int& argc) {
		NumVar nv((*vars[0])->get() + (*vars[1])->get());
		**vars[0] = nv;
		return reg;
	}

	variable& scast(lwc::variable** vars, variable& reg, const int& argc) {
		*vars[0] = (LASTVariable*)(*vars[1]);
		return *vars[0];
	}

	variable& it_elast(variable** vars, variable& reg, const int& argc)
	{
		*vars[0] = (LASTVariable*)(*vars[1]);
		return *vars[0];
	}
	variable& bad_name(variable** vars, variable& reg, const int& argc)
	{
		std::cout << "A bad name was used somewhere in your code." << std::endl;
		exit(lwc::BAD_NAME_INTERNAL_ERR);
	}
}