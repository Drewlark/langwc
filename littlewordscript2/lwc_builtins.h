#include <vector>
#include <iostream>
#include <iomanip>
#ifndef LWC_BUILTINS
#define LWC_BUILTINS
using namespace std;

namespace lwc {
	int add(const varset &vars) {
		return *vars[0] + *vars[1];
	}

	int sub(const varset &vars) {
		return *vars[0] - *vars[1];
	}

	int assign(const varset &vars) {
		*vars[0] = *vars[1];
		return 1;
	}

	int print(const varset &vars) {
		cout << ">:  ";
		for (variable var : vars) {
			cout << *var << " ";
		}
		cout << endl;
		return 0;
	}
}
#endif //!LWC_BUILTINS