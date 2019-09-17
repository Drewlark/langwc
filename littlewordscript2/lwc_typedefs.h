#include <memory>
#include <vector>
#include <sstream>
#include <deque>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include "lwc_prims.h"
#ifndef LWC_TYPEDEF
#define LWC_TYPEDEF

namespace lwc {
	class Line;
	class Evaluator;
	template<typename T> class TypeImpl;
	struct NumVar;
	struct BaseVariable;

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

	typedef std::shared_ptr<NumVar> n_variable;
	typedef std::vector<variable> varset;

	
	template<typename T> class TypeImpl : public RegisterType
	{
	public:
		virtual void* allocate()const { return new T; }
		virtual void* cast(void* obj)const { return static_cast<T*>(obj); }
		virtual int size()const { return sizeof(T); }
	};


	//builtin_func definition moved to lwc_RegisterStore
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
		builtin_func opfunc;
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

	class BaseFunction {
		bool is_builtin = true;
	public:
		BaseFunction(std::unordered_map<std::string, lwc::variable>& varmap, std::string name) {
			if (varmap.count(name) > 0) {
				is_builtin = false;
			}

		}
	};

	class TokenQueue { //TokenQueue is an object which represents the end result of a lexed line. The constructor is LWC's lexer
		static enum class QState { def, op, num, elastic };
		std::deque<lwc::ParseToken> data;
		std::stack<lwc::ParseToken*> func_stack;
		int paren_depth = 0;
		void add_unknown(std::string& unk, QState& qs);
		bool checkparens(std::string& tmp, char& c, QState& qs, bool& pcheck);
	public:
		bool brace_end = false;
		TokenQueue(std::string s);
		ParseToken pop();
		bool empty();
	};

	class LAST;

	class LineNode {
		std::vector<LineNode*> branches;
	public:
		builtin_func func = nullptr;
		std::vector<LAST> output_block;

		bool is_rval = true;
		variable var;
		variable* lvar = nullptr;
		bool is_leaf = false;

		variable* arg_arr = nullptr;
		int sz = 0;
		RegisterType* rt = nullptr;
		variable rgstr;


		void fit_args();
		void fit_register();

		LineNode(builtin_func _func, RegisterType* _rt, std::vector<LineNode*> _branches = {}, bool rval = false);

		LineNode(variable _var, RegisterType* _rt) : var(_var), rt(_rt) { is_leaf = true; }
		LineNode(variable* _lvar, RegisterType* _rt) : lvar(_lvar), rt(_rt) { is_leaf = true; is_rval = false; }
		LineNode() { is_leaf = true; }
		~LineNode() { delete[] arg_arr; }

		void add_branch(LineNode* ln);

		variable& get_leaf() {return is_rval ? var : *lvar;}

		LineNode* get_branch(const int& index);
	};

	struct LAST { //"Line" abstract syntax tree
		LineNode* root;
		uint8_t block_ends = 0;
		uint8_t block_starts = 0;
		LineNode* block_node = nullptr;
		LAST(std::queue<ParseToken> tq, std::unordered_map<std::string, lwc::variable>& global);
		~LAST() {}
	};
	typedef std::vector<LAST> block_func;



	class LASTVariable : public BaseVariable //variable wrapping a vector of type LAST
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

	struct Scope {
		//vector<
	};
}




#endif // !H_LWC_LINE