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

	

	class RegisterType
	{
	public:
		virtual ~RegisterType() {}
		virtual void* allocate()const = 0;
		virtual void* cast(void* obj)const = 0;
		virtual int size()const = 0;
	};

	template<typename T> class TypeImpl : public RegisterType
	{
	public:
		virtual void* allocate()const { return new T; }
		virtual void* cast(void* obj)const { return static_cast<T*>(obj); }
		virtual int size()const { return sizeof(T); }
	};

	using builtin_func = variable  (*)(variable* vars, const variable &reg, const int& argc);

	typedef std::shared_ptr<void> soft_typed;
	//builtin_func definition moved to lwc_RegisterStore
	
}




#endif // !H_LWC_LINE