#include <memory>
#include <vector>
#include <initializer_list>
#ifndef LWC_TYPEDEF
#define LWC_TYPEDEF

namespace lwc {
	class Line;
	class Evaluator;

	struct base_variable {
		virtual long get() const { return 0; }
		base_variable() {};
		virtual base_variable operator+(const base_variable& bv) { return base_variable(); }
		virtual base_variable operator-(const base_variable& bv) { return base_variable(); }
		virtual base_variable operator*(const base_variable& bv) { return base_variable(); }
		virtual base_variable operator/(const base_variable& bv) { return base_variable(); }
		virtual void operator=(const base_variable& bv) {}
	};

	struct line_var : base_variable {
		lwc::Evaluator& eval;
		lwc::Line& line;
		line_var(lwc::Evaluator& _eval, Line &l) : eval(_eval), line(l) {}
		long get() const {};
	};

	struct num_var : base_variable {
	protected:
		long n;
	public:
		num_var(long _n) : n(_n) {};
		long get() const { return n; };
		operator long() const { return n; }
		base_variable operator+(const base_variable& bv) { return num_var(n + bv.get());}
		base_variable operator-(const base_variable& bv) { return num_var(n - bv.get()); }
		base_variable operator*(const base_variable& bv) { return num_var(n * bv.get()); }
		base_variable operator/(const base_variable& bv) { return num_var(n / bv.get()); }
		virtual void operator=(const base_variable& bv) { n = bv.get(); }
	};

	typedef std::shared_ptr<base_variable> variable;
	typedef std::shared_ptr<num_var> n_variable;
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

	using builtin_func = long(*)(lwc::varset&);

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