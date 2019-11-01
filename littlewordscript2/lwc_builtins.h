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

	#define builtin_header(FUNC_NAME) variable& FUNC_NAME (variable** vars, variable& reg, const int& argc)

	//variable mk_num_var() {};

	builtin_header(add);
	builtin_header(sub);
	builtin_header(mult);
	builtin_header(div);
	builtin_header(while_loop);
	builtin_header(assign);
	builtin_header(refassign);
	builtin_header(is_lessthan);
	builtin_header(is_greaterthan);
	builtin_header(print);
	builtin_header(incrementby);
	builtin_header(scast);
	builtin_header(it_elast);
	builtin_header(bad_name);
	builtin_header(nihil);
	builtin_header(sexecute);


	
}
#endif //!LWC_BUILTINS