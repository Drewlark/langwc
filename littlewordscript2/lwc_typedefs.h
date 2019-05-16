#include <memory>
#include <vector>

#ifndef LWC_TYPEDEF
#define LWC_TYPEDEF
namespace lwc {
	struct raw_variable {
		//will contain either a numerical or Line dat
	};


	typedef std::shared_ptr<long> variable;
	typedef std::vector<variable> varset;



	struct static_varset {

	public:
		int n;
		variable* data;
		static_varset() : n(0) {
			data = new variable[0];
		}

		static_varset(varset _vars, const int _n) : n(_n) {
			data = new variable[n];
			for (int i = 0; i < n; ++i) { //copy data
				data[i] = _vars[i];
			}
		}

		static_varset(variable _vars[], const int _n) : n(_n) {
			data = new variable[n];
			for (int i = 0; i < n; ++i) { //copy data
				data[i] = _vars[i];
			}
		}

		static_varset(const static_varset& sv) {
			n = sv.n;
			data = new variable[n];
			for (int i = 0; i < n; ++i) { //copy data
				data[i] = sv.data[i];
			}
		}

		~static_varset() {
			//Nothing needed here due to the fact that data is entirely composed of smart pointers
		}

		variable& operator[](const int i) {
			return data[i]; //no memory protection here.
		}

		void operator=(const static_varset& sv) {
			n = sv.n;
			data = new variable[sv.n];
			for (int i = 0; i < sv.n; ++i) { //copy data
				data[i] = sv.data[i];
			}
		}


	};

	using builtin_func = long(*)(static_varset&);
}
#endif