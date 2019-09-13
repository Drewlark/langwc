#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>
#include "lwc_typedefs.h"
#ifndef LWC_BUILTINS
#define LWC_BUILTINS
using namespace std;

// #define make_num 

namespace lwc {

	//variable mk_num_var() {};

	variable add(variable* vars, const variable& reg, const int& argc);

	variable sub(variable* vars, const variable& reg, const int& argc);

	variable mult(variable* vars, const variable& reg, const int& argc);

	variable div(variable* vars, const variable& reg, const int& argc);

	variable while_loop(variable* vars, const variable& reg, const int& argc);

	variable assign(variable* vars, const variable& reg, const int& argc);

	variable refassign(variable* vars, const variable& reg, const int& argc);

	variable is_lessthan(variable* vars, const variable& reg, const int& argc);

	variable is_greaterthan(variable* vars, const variable& reg, const int& argc);

	variable print(variable* vars, const variable& reg, const int& argc);

	variable incrementby(lwc::variable* vars, const variable& reg, const int& argc);

	variable scast(lwc::variable* vars, const variable& reg, const int& argc);

	
}
#endif //!LWC_BUILTINS