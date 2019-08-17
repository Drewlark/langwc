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

	using builtin_func = variable(*)(variable*, const int& argc);
	
}




#endif // !H_LWC_LINE