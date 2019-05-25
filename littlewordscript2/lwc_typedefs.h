#include <memory>
#include <vector>
#include <initializer_list>
#ifndef LWC_TYPEDEF
#define LWC_TYPEDEF
class Line;
class Evaluator;
namespace lwc {

	struct raw_variable {
		virtual long get() {}
		raw_variable() {};
	};

	/*struct line_var : raw_variable {
		Evaluator& eval;
		Line& line;
		line_var(Evaluator& _eval, Line &l) : eval(_eval), line(l) {
		}
		long get() {
			eval.single_eval(line);
		}
	};*/

	struct num_var : raw_variable {
		long n;
		num_var(long _n) : n(_n) {};
		long get() { return n; }
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
	struct Evaluator {
		long last_eval = 0;

		Evaluator() {};

		const long& evaluate(std::vector<Line>& linevec)
		{

			for (Line& ln : linevec) {
				if (ln.request_last) {
					*(ln.vars)[ln.getN() - 1] = last_eval; //set last element to the result of last operation
				}
				last_eval = ln.func(ln.vars);
				if (ln.linked_lines != nullptr && ln.linked_lines->size() > 0 && last_eval) {

					if (ln.loop) {
						while (ln.func(ln.vars)) {
							evaluate((*ln.linked_lines));
						}
					}
					else {
						evaluate((*ln.linked_lines));
					}
				}
			}
			return last_eval;
		}

		const long& single_eval(Line &ln) {
			if (ln.request_last) {
				*(ln.vars)[ln.getN() - 1] = last_eval; //set last element to the result of last operation
			}
			last_eval = ln.func(ln.vars);
			if (ln.linked_lines != nullptr && ln.linked_lines->size() > 0 && last_eval) {

				if (ln.loop) {
					while (ln.func(ln.vars)) {
						evaluate((*ln.linked_lines));
					}
				}
				else {
					evaluate((*ln.linked_lines));
				}
			}
			return last_eval;
		}
	};
}

//typedef std::unordered_map<std::string, long> dataset;




#endif // !H_LWC_LINE