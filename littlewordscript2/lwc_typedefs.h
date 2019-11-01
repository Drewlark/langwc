#include <memory>
#include <vector>
#include <sstream>
#include <deque>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <type_traits> // For ensuring certain templated functions only use template classes deriving from BaseVariable (defined in lwc_prims.h)
#include <iostream>
#include <set>
#include "lwc_prims.h"
#include "lwc_Lexing.h"
#ifndef LWC_TYPEDEF
#define LWC_TYPEDEF

namespace lwc {
	class Line;
	template<typename T> class TypeImpl;

	struct NumVar : BaseVariable {
	private:
		const static RegisterType* typei;
	protected:
		long n;
	public:
		//const static RegisterType const* typei;
		//static const RegisterType const* typei = &NUM_TYPEI;
		NumVar() : n(0) {};
		NumVar(BaseVariable bv) : n(bv.get()) {};
		NumVar(long _n) : n(_n) {};
		long get()  { return n; };
		void* get_vp(void*& reg) { *(long*)reg = this->get();  return reg; }
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
		std::set<lwc::Keywords> keywords;

		ParseToken(std::string _val, TokenType _tt, RegisterType* _rt, int _precedence = 0, bool _leftassoc = 0, bool _rval = false);
		ParseToken(OperatorIdentity oid) : tt(TokenType::op), precedence(oid.precedence), leftassoc(oid.leftassoc), opfunc(oid.fnc), rval(oid.rval), rt(oid.rt), val(oid.debug_name) {}; //No value is possible here because the string value is irrelevant to an op
		bool operator<(const ParseToken& pt) 
		{ return precedence < pt.precedence; }
		bool operator>(const ParseToken& pt) 
		{ return precedence > pt.precedence; }
		bool operator>=(const ParseToken& pt) 
		{ return precedence >= pt.precedence; }
		bool operator<=(const ParseToken& pt) 
		{ return precedence <= pt.precedence; }
	};

	class TokenQueue { //TokenQueue is an object which represents the end result of a lexed line. The constructor is LWC's lexer
		enum class QState { def, op, num, elastic };
		std::deque<lwc::ParseToken> data;
		std::stack<lwc::ParseToken*> func_stack;
		std::deque<lwc::Keywords> keyword_queue;
		int paren_depth = 0;
		void check_for_argness(); //Checks if func stack is not empty, and then increments argn for that func if not. Only call this when var-like token is found.
		void add_unknown(std::string& unk, QState& qs);
		bool checkparens(std::string& tmp, char& c, QState& qs, bool& pcheck);
	public:
		bool brace_end = false;
		TokenQueue(bool _brace_end) : brace_end(_brace_end) {}
		TokenQueue(std::string s);
		ParseToken pop();
		void void_pop() { data.pop_front();}
		void push(ParseToken pt) { data.push_back(pt); }
		ParseToken& front() { return data.front(); }
		void clear() { data.clear(); }
		bool empty();
	};

	struct LAST;
	class LineNode;

	typedef std::vector<unsigned int> branches_t;
	typedef std::vector<LineNode*> master_lns;
	typedef std::vector<bool> leaf_states_t;

	class LineNode {
		friend struct LAST;
		branches_t branches;
		std::shared_ptr<master_lns> const master;
		static master_lns* LEAF_MASTER;
		leaf_states_t leaf_states;
	public:
		builtin_func func = nullptr;
		std::vector<LAST> output_block;

		bool is_rval = true;
		union {
			variable var = nullptr;
			variable* lvar;
		} data;
		bool is_leaf = false;
		std::string name; // important for debugging

		variable** arg_arr = nullptr;
		int sz = 0;
		RegisterType* rt = nullptr;
		variable rgstr = nullptr;



		void fit_args();
		void fit_register();
		void fit_leaf_states();
		bool get_leaf_state(const int& i);

		LineNode(std::shared_ptr<master_lns> _master, builtin_func _func, RegisterType* _rt, branches_t _branches = {}, bool rval = false);

		LineNode(variable _var, RegisterType* _rt) :rt(_rt), master(LEAF_MASTER) { data.var = _var; is_leaf = true; }
		LineNode(variable* _lvar, RegisterType* _rt, std::string _name) : rt(_rt), name(_name), master(LEAF_MASTER) { data.lvar = _lvar; is_leaf = true; is_rval = false; }
		LineNode() : master(LEAF_MASTER) { is_leaf = true;}
		~LineNode() { delete[] arg_arr;}

		void add_branch(LineNode* ln);

		variable* get_leaf() {return is_rval ? &data.var : data.lvar;}

		LineNode* get_branch(const int& index);
	};

	class Scope;

	struct LAST { //"Line" abstract syntax tree
		std::shared_ptr<master_lns> master = std::make_shared<master_lns>();
		LineNode* root = nullptr;
		uint8_t block_ends = 0;
		uint8_t block_starts = 0;
		LineNode* block_node = nullptr;
		std::vector<LineNode*> breadthwise;
		void rebuild();
		LAST(TokenQueue tq, Scope& scope);
		//LAST(const LAST& last2);
#if defined(SHOW_LAST_DEL)		
		~LAST() {

			std::cout << "LAST destroyed with default destructor\n";

		}
#endif
	};
	typedef std::vector<LAST> CodeBlock;

	class Scope {
		std::unordered_map<std::string, variable> names;
	public:
		Scope* parent = nullptr;
		Scope(Scope* _parent = nullptr) : parent(_parent) {};

		variable* find_name_upward(const std::string& name)
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

		bool is_var_local(std::string s) {
			return names.count(s);
		}

		template <class T = NumVar> // T represents type to initialize to, should always be derivative of BaseVariable
		variable * handle_name(const std::string & name) {
			static_assert(std::is_base_of<BaseVariable, T>::value, "T must derive from lwc::BaseVariable");
			if (names.count(name)) {
				return &names[name];
			}
			else {
				names[name] = new T();
				return &names[name];
			}
		}
		int count(std::string& name) { return names.count(name); }
		variable* operator[](std::string& key) { return find_name_upward(key); }

	};

	class LASTVariable : public BaseVariable // `` backtick expression variable (single LAST/Line)
	{
		std::shared_ptr<LineNode> linenode;
		static const RegisterType* typei;
	public:
		LASTVariable(LineNode* _linenode) : linenode(_linenode) {}
		LASTVariable() {}
		long get();
		void* get_vp(void*& reg) { *(long*)reg = this->get();  return reg; }
		RegisterType const* const get_typei();
	};

	class CodeBlockVariable : public BaseVariable //variable wrapping a vector of type LAST
	{
		CodeBlock code_block;
		static const RegisterType* typei;
	public:
		CodeBlockVariable(CodeBlock& _code_block) : code_block(_code_block) { std::cout << "CBV created\n"; }
		CodeBlockVariable() {}
		long get();
		void* get_vp(void*& reg) { *(long*)reg = this->get();  return reg; }
		RegisterType const* const get_typei();
	};


}




#endif // !H_LWC_LINE