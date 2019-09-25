#include "lwc_builtins.h"
#include "lwc_typedefs.h"
#include "lwc_err_codes.h"
#include <thread>
#include <future>

namespace lwc {

	long get_from_var(variable* var) {
		return (*var)->get();
	}

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
	
	variable& nihil(variable** vars, variable& reg, const int& argc)
	{
		return reg;
	}
	
	variable& sexecute(variable** vars, variable& reg, const int& argc)
	{
		if (argc == 1) {
			NumVar nv((*vars[0])->get());
			*reg = nv;
			return reg;
		}
		else {
			std::vector<std::future<long>> sexys;
			for (int i = 0; i < argc; i++) {
				sexys.push_back(std::async(&get_from_var, vars[i]));
			}
			int weird = 0;
			for (int i = 0; i < argc; i++) {
				weird ^= sexys[i].get(); //TODO rather than XOR outputs, should go into an ArrayVariable of some sort
			}
			NumVar nv(weird);
			*reg = nv;
		}
		return reg;
	}
}