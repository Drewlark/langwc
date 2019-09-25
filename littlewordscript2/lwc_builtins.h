#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>
#include "lwc_typedefs.h"
#include "lwc_err_codes.h"
#ifndef LWC_BUILTINS
#define LWC_BUILTINS
using namespace std;

// #define make_num 

namespace lwc {

	//variable mk_num_var() {};

	variable& add(variable** vars, variable& reg, const int& argc);

	variable& sub(variable** vars, variable& reg, const int& argc);

	variable& mult(variable** vars, variable& reg, const int& argc);

	variable& div(variable** vars, variable& reg, const int& argc);

	variable& while_loop(variable** vars, variable& reg, const int& argc);

	variable& assign(variable** vars, variable& reg, const int& argc);

	variable& refassign(variable** vars, variable& reg, const int& argc);

	variable& is_lessthan(variable** vars, variable& reg, const int& argc);

	variable& is_greaterthan(variable** vars, variable& reg, const int& argc);

	variable& print(variable** vars, variable& reg, const int& argc);

	variable& incrementby(variable** vars, variable& reg, const int& argc);

	variable& scast(variable** vars, variable& reg, const int& argc);

	variable& it_elast(variable** vars, variable& reg, const int& argc);

	variable& bad_name(variable** vars, variable& reg, const int& argc);

	variable& nihil(variable** vars, variable& reg, const int& argc);

	variable& sexecute(variable** vars, variable& reg, const int& argc);


	
}
#endif //!LWC_BUILTINS