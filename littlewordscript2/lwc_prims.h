#include <sstream>
#include <string>

// Debug definitions
// #define PRINT_LVAL_INFO

#ifndef H_LWC_PRIMS
#define H_LWC_PRIMS
namespace lwc {
	class RegisterType
	{
	public:
		virtual ~RegisterType() {}
		virtual void* allocate()const = 0;
		virtual void* cast(void* obj)const = 0;
		virtual int size()const = 0;
	};

	struct BaseVariable {
	private:
	public:
		virtual long get() { return 0; }
		virtual void* get_vp(void*& reg) { *(long*)reg = this->get();  return reg; }
		BaseVariable() {};
		virtual BaseVariable operator+(BaseVariable& bv) { return BaseVariable(); }
		virtual BaseVariable operator-(BaseVariable& bv) { return BaseVariable(); }
		virtual BaseVariable operator*(BaseVariable& bv) { return BaseVariable(); }
		virtual BaseVariable operator/(BaseVariable& bv) { return BaseVariable(); }
		virtual void operator=(BaseVariable& bv) {}
		virtual void operator=(const BaseVariable& bv) {}
		virtual RegisterType const* const get_typei() { return NULL; }
		virtual std::stringstream repr() { std::stringstream ss; return ss; };
		virtual ~BaseVariable() {}
	};

	typedef BaseVariable* variable;
	using builtin_func = variable&(*)(variable** vars, variable& reg, const int& argc);

	struct OperatorIdentity
	{
		lwc::builtin_func fnc = nullptr;
		bool leftassoc = false;
		int8_t precedence = 0;
		bool rval = false;
		RegisterType* rt = nullptr;
		std::string debug_name;
		OperatorIdentity(lwc::builtin_func _fnc, bool _leftassoc,
			int8_t _precedence, bool _rval = false, 
			RegisterType* _rt = nullptr, std::string _debug_name = "") : fnc(_fnc), leftassoc(_leftassoc),
			precedence(_precedence), rval(_rval), rt(_rt), debug_name(_debug_name) {};
		OperatorIdentity() { delete rt; };
		~OperatorIdentity() {};
	};

	struct BuiltInIdentity {
		lwc::builtin_func fnc = nullptr;
		int arg_count = 0;
		bool rval = false;
		RegisterType* rt = nullptr;
		BuiltInIdentity(lwc::builtin_func _fnc, RegisterType* _rt, int _arg_count = 0, bool _rval = false) : fnc(_fnc), arg_count(_arg_count), rval(_rval), rt(_rt) {}
		BuiltInIdentity() {};
		~BuiltInIdentity() { };
	};
}

#endif // !H_LWC_PRIMS
