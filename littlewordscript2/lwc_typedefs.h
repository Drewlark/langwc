#include <memory>
#include <vector>
#include <sstream>
#include <deque>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <type_traits> // For ensuring certain templated functions only use template classes deriving from BaseVariable (defined in lwc_prims.h)
#include "lwc_prims.h"
#ifndef LWC_TYPEDEF
#define LWC_TYPEDEF

namespace lwc {
	class Line;
	template<typename T> class TypeImpl;

	struct NumVar : BaseVariable {
	private:
		const static RegisterType const* typei;
	protected:
		long n;
	public:
		//const static RegisterType const* typei;
		//static const RegisterType const* typei = &NUM_TYPEI;
		NumVar() : n(0) {};
		NumVar(BaseVariable bv) : n(bv.get()) {};
		NumVar(long _n) : n(_n) {};
		long get()  { return n; };
		operator long() const { return n; }
		BaseVariable operator+( BaseVariable& bv) { return NumVar(n + bv.get());}
		BaseVariable operator-( BaseVariable& bv) { return NumVar(n - bv.get()); }
		BaseVariable operator*( BaseVariable& bv) { return NumVar(n * bv.get()); }
		BaseVariable operator/( BaseVariable& bv) { return NumVar(n / bv.get()); }
		RegisterType const* const get_typei() { return typei; }
		void operator=( BaseVariable& bv) { n = bv.get(); }
		std::stringstream repr() { std::stringstream ss; ss << n; return ss;}
	};

	typedef std::vector<variable> varset;

	
	template<typename T> class TypeImpl : public RegisterType
	{
	public:
		virtual void* allocate()const { return new T; }
		virtual void* cast(void* obj)const { return static_cast<T*>(obj); }
		virtual int size()const { return sizeof(T); }
	};


	const static TypeImpl<BaseVariable> BASE_TYPEI;
	const static TypeImpl<NumVar> NUM_TYPEI;

	enum class TokenType { num, op, name, lparen, rparen, func, comma, elastic };

	class ParseToken
	{
	public:
		std::string val;
		TokenType tt;
		int precedence; //meaningless if tt is not TokenType::op
		bool leftassoc = false;
		bool brace_start = false;
		bool brace_end = false;
		builtin_func opfunc = nullptr;
		int argn = 0;
		bool rval = false;
		RegisterType* rt = nullptr;

		ParseToken(std::string _val, TokenType _tt, RegisterType* _rt, int _precedence = 0, bool _leftassoc = 0, bool _rval = false);
		ParseToken(OperatorIdentity oid) : tt(TokenType::op), precedence(oid.precedence), leftassoc(oid.leftassoc), opfunc(oid.fnc), rval(oid.rval), rt(oid.rt) {}; //No value is possible here because the string value is irrelevant to an op
		bool operator<(const ParseToken& pt) { return precedence < pt.precedence; }
		bool operator>(const ParseToken& pt) { return precedence > pt.precedence; }
		bool operator>=(const ParseToken& pt) { return precedence >= pt.precedence; }
		bool operator<=(const ParseToken& pt) { return precedence <= pt.precedence; }
	};

	class TokenQueue { //TokenQueue is an object which represents the end result of a lexed line. The constructor is LWC's lexer
		static enum class QState { def, op, num, elastic };
		std::deque<lwc::ParseToken> data;
		std::stack<lwc::ParseToken*> func_stack;
		int paren_depth = 0;
		void check_for_argness(); //Checks if func stack is not empty, and then increments argn for that func if not. Only call this when var-like token is found.
		void add_unknown(std::string& unk, QState& qs);
		bool checkparens(std::string& tmp, char& c, QState& qs, bool& pcheck);
	public:
		bool brace_end = false;
		TokenQueue(std::string s);
		ParseToken pop();
		bool empty();
	};

	struct LAST;

	class LineNode {
		std::vector<LineNode*> branches;
	public:
		builtin_func func = nullptr;
		std::vector<LAST> output_block;

		bool is_rval = true;
		variable var = nullptr;
		variable* lvar = nullptr;
		bool is_leaf = false;

		variable** arg_arr = nullptr;
		int sz = 0;
		RegisterType* rt = nullptr;
		variable rgstr = nullptr;


		void fit_args();
		void fit_register();

		LineNode(builtin_func _func, RegisterType* _rt, std::vector<LineNode*> _branches = {}, bool rval = false);

		LineNode(variable _var, RegisterType* _rt) : var(_var), rt(_rt) { is_leaf = true; }
		LineNode(variable* _lvar, RegisterType* _rt) : lvar(_lvar), rt(_rt) { is_leaf = true; is_rval = false; }
		LineNode() { is_leaf = true; }
		~LineNode() { delete[] arg_arr; }

		void add_branch(LineNode* ln);

		variable* get_leaf() {return is_rval ? &var : lvar;}

		LineNode* get_branch(const int& index);
	};

	class Scope {
		//Currently unused
		std::unordered_map<std::string, variable> names;
	public:
		Scope* parent = nullptr;
		Scope(Scope* _parent = nullptr) : parent(_parent) {};
		
		variable* find_name_upward(const std::string& const name)
		{
			if (names.count(name)) {
				return &names[name];
			}
			else if (parent) {
				return parent->find_name_upward(name);
			}
			else {
				return nullptr;
			}
		}
		
		template <class T = NumVar> // T represents type to initialize to, should always be derivative of BaseVariable
		variable* handle_name(const std::string& const name) {
			static_assert(std::is_base_of<BaseVariable, T>::value, "T must derive from lwc::BaseVariable");
			if (names.count(name)) {
				return &names[name];
			}
			else {
				names[name] = new T();
				return &names[name];
			}
		}

		int count(const std::string& const name) { return names.count(name); }
		variable* operator[](const std::string& const key) { return find_name_upward(key); }

	};

	struct LAST { //"Line" abstract syntax tree
		LineNode* root = nullptr;
		uint8_t block_ends = 0;
		uint8_t block_starts = 0;
		LineNode* block_node = nullptr;
		LAST(std::queue<ParseToken> tq, Scope& scope);
		~LAST() {}
	};
	typedef std::vector<LAST> block_func;



	class LASTVariable : public BaseVariable // `` backtick expression variable (single LAST/Line)
	{
		std::shared_ptr<LineNode> linenode;
		static const RegisterType const* typei;
	public:
		LASTVariable(LineNode* _linenode) : linenode(_linenode) {}
		LASTVariable() {}
		long get();
		RegisterType const* const get_typei();
	};

	class CodeBlockVariable : public BaseVariable //variable wrapping a vector of type LAST
	{
		block_func code_block;
		static const RegisterType const* typei;
	public:
		CodeBlockVariable(block_func& _code_block) : code_block(_code_block) {}
		CodeBlockVariable() {}
		long get();
		RegisterType const* const get_typei();
	};


}




#endif // !H_LWC_LINE