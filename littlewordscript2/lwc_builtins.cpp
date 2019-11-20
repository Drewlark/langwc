#include "lwc_builtins.h"
#include "lwc_typedefs.h"
#include "lwc_err_codes.h"
#include <thread>
#include <future>

namespace lwc {



	long get_from_var(variable* var) {
		return (*var)->get();
	}

	builtin_header(add) {
		NumVar nv((*vars[0])->get() + (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	builtin_header(sub) {
		NumVar nv((*vars[0])->get() - (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	builtin_header(mult) {
		NumVar nv((*vars[0])->get() * (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	builtin_header(div) {
		NumVar nv((*vars[0])->get() / (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	builtin_header(while_loop) {
		while ((*vars[0])->get()) {
			(*vars[1])->get();
		}
		return *vars[0];
	}

	builtin_header(assign) {
		**vars[0] = **vars[1];
		return *vars[1];
	}

	builtin_header(refassign) {
		delete *vars[0];
		*vars[0] = *vars[1];
		return *vars[1];
	}

	builtin_header(is_lessthan) {
		NumVar nv((*vars[0])->get() < (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	builtin_header(is_greaterthan) {
		NumVar nv((*vars[0])->get() > (*vars[1])->get());
		*reg = nv;
		return reg;
	}

	builtin_header(print) {
		for (int i = 0; i < argc; i++) {
			std::cout << (*vars[i])->get() << " ";
		}
		std::cout << std::endl;
		return reg;
	}

	builtin_header(incrementby) {
		NumVar nv((*vars[0])->get() + (*vars[1])->get());
		**vars[0] = nv;
		return reg;
	}

	builtin_header(scast) {
		*vars[0] = (LASTVariable*)(*vars[1]);
		return *vars[0];
	}

	builtin_header(it_elast)
	{
		delete* vars[0];
		*vars[0] = (LASTVariable*)(*vars[1]);
		return *vars[0];
	}

	builtin_header(bad_name)
	{
		std::cout << "A bad name was used somewhere in your code." << std::endl;
		exit(lwc::BAD_NAME_INTERNAL_ERR);
	}
	
	builtin_header(nihil)
	{
		//std::cout << *vars[0] << " " << *vars[1];
		return reg;
	}
	
	builtin_header(sexecute)
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
				weird ^= sexys[i].get(); // TODO rather than XORing outputs, should go into an ArrayVariable of some sort
			}
			NumVar nv(weird);
			*reg = nv;
		}
		return reg;
	}
}