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
		virtual long get() { return 0; }
		BaseVariable() {};
		virtual BaseVariable operator+( BaseVariable& bv) { return BaseVariable(); }
		virtual BaseVariable operator-( BaseVariable& bv) { return BaseVariable(); }
		virtual BaseVariable operator*( BaseVariable& bv) { return BaseVariable(); }
		virtual BaseVariable operator/( BaseVariable& bv) { return BaseVariable(); }
		virtual void operator=(BaseVariable& bv) {}
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
		long get()  { return n; };
		operator long() const { return n; }
		BaseVariable operator+( BaseVariable& bv) { return NumVar(n + bv.get());}
		BaseVariable operator-( BaseVariable& bv) { return NumVar(n - bv.get()); }
		BaseVariable operator*( BaseVariable& bv) { return NumVar(n * bv.get()); }
		BaseVariable operator/( BaseVariable& bv) { return NumVar(n / bv.get()); }
		void operator=( BaseVariable& bv) { n = bv.get(); }
		std::stringstream repr() { std::stringstream ss; ss << n; return ss;}
	};
	typedef std::shared_ptr<BaseVariable> variable;
	typedef std::shared_ptr<NumVar> n_variable;
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
	
	struct _AllVal {
		variable lval;
		BaseVariable rval;
		_AllVal() : lval{} { }
		~_AllVal() {};
	};

	struct AllVal {
		
		int type = 0;
		_AllVal _av;
		AllVal(variable _lval) { 
			type = 0; 
			_av.lval = _lval; 
		}
		AllVal(BaseVariable _rval) {
			type = 1; 
			_av.rval = _rval; 
		}
		AllVal() {};
		
		AllVal& operator=(const AllVal& av) 
		{
			type = av.type;
			if (av.type) {
				_av.rval = av._av.rval;
				return *this;
			}
			_av.lval = av._av.lval;
			return *this;
		}

		long get() {
			if (type)
				return _av.rval.get();
			return _av.lval->get();
		}

		BaseVariable & getbv() {
			if (type)
				return _av.rval;
			return *_av.lval;
		}

		variable &getv() {
			if (type) {
				_av.lval = std::make_shared<BaseVariable>(_av.rval);
				type = 0;
			}
			return _av.lval;
		}
	};

	/*using builtin_lfunc = variable(*)(AllVal*, const int &argc);
	using builtin_rfunc = BaseVariable(*)(AllVal*, const int& argc);

	union builtin_func {
		builtin_lfunc lfunc = nullptr;
		builtin_rfunc rfunc;
		builtin_func(builtin_lfunc _lfunc) : lfunc(_lfunc) {}
		builtin_func(builtin_rfunc _rfunc) : rfunc(_rfunc) {}
		builtin_func() {};
	};*/

	using builtin_func = variable(*)(variable*, const int& argc);


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