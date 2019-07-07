#include <memory>
#include <vector>
#include <sstream>
#include <initializer_list>
#ifndef LWC_TYPEDEF
#define LWC_TYPEDEF

namespace lwc {
	class Line;
	class Evaluator;

	struct NumVar;

	struct BaseVariable {
		virtual long get() const { return 0; }
		BaseVariable() {};
		virtual BaseVariable operator+(const BaseVariable& bv) { return BaseVariable(); }
		virtual BaseVariable operator-(const BaseVariable& bv) { return BaseVariable(); }
		virtual BaseVariable operator*(const BaseVariable& bv) { return BaseVariable(); }
		virtual BaseVariable operator/(const BaseVariable& bv) { return BaseVariable(); }
		virtual void operator=(const BaseVariable& bv) {}
		virtual std::stringstream repr() { std::stringstream ss; return ss; };
	};

	struct NumVar : BaseVariable {
	protected:
		long n;
	public:
		NumVar() : n(0) {};
		NumVar(BaseVariable bv) : n(bv.get()) {};
		NumVar(long _n) : n(_n) {};
		long get() const { return n; };
		operator long() const { return n; }
		BaseVariable operator+(const BaseVariable& bv) { return NumVar(n + bv.get());}
		BaseVariable operator-(const BaseVariable& bv) { return NumVar(n - bv.get()); }
		BaseVariable operator*(const BaseVariable& bv) { return NumVar(n * bv.get()); }
		BaseVariable operator/(const BaseVariable& bv) { return NumVar(n / bv.get()); }
		void operator=(const BaseVariable& bv) { n = bv.get(); }
		std::stringstream repr() { std::stringstream ss; ss << n; return ss;}
	};
	typedef BaseVariable* variable;
	typedef NumVar* n_variable;
	typedef std::vector<variable> varset;

	/*struct block_var : base_variable {
		vector<LAST> line_bois;
	};*/

	struct static_varset { //deprecated

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

	using builtin_func = BaseVariable*(*)(lwc::varset&);
	class Line {
		int n;
	public:
		lwc::static_varset vars;
		lwc::builtin_func func;
		bool request_last = false;
		bool loop = false;
		std::vector<Line>* linked_lines = nullptr;

		Line(std::vector<lwc::variable> _vars, lwc::builtin_func _func, bool _reqlast = false);

		Line(lwc::variable _var, lwc::builtin_func _func, bool _reqlast = false);

		~Line() { /*delete vars;*/ }

		int getN() {
			return n;
		}
	};

	typedef std::vector<Line> function;
	typedef std::shared_ptr<std::vector<Line>> codeblock;
	
}

//typedef std::unordered_map<std::string, long> dataset;




#endif // !H_LWC_LINE