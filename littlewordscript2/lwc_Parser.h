#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <deque>
#include <iterator>
#include <tuple>
#include <deque>
#include <memory>
#include <array>
#include "lwc_typedefs.h"
#include "lwc_builtins.h"
#include "lwc_RegisterStore.h"


#ifndef H_PARSER
#define H_PARSER
//TODO break this up into multiple files...
namespace lwc {
	struct OperatorIdentity
	{
		lwc::builtin_func fnc;
		bool leftassoc = false;
		int8_t precedence = 0;
		bool rval = false;
		RegisterType* rt = nullptr;
		OperatorIdentity(lwc::builtin_func _fnc, bool _leftassoc,
			int8_t _precedence, bool _rval=false, RegisterType *_rt = nullptr) : fnc(_fnc), leftassoc(_leftassoc), precedence(_precedence), rval(_rval),rt(_rt) {};
		OperatorIdentity() {};
		~OperatorIdentity() {};
	};

	static const std::unordered_map<std::string, OperatorIdentity> op_ids = {
		{"+", OperatorIdentity(builtin_func(lwc::add), 0, 0, true, new TypeImpl<NumVar>())}, //TODO temporary solution to statically define return types.
		{"*", OperatorIdentity(builtin_func(lwc::mult), 0, 1, true, new TypeImpl<NumVar>())},
		{"-", OperatorIdentity(builtin_func(lwc::sub), 1, 0, true, new TypeImpl<NumVar>())},
		{"/", OperatorIdentity(builtin_func(lwc::div), 1, 1, true, new TypeImpl<NumVar>())},
		{"=", OperatorIdentity(builtin_func(lwc::assign), 0, -2, true, new TypeImpl<NumVar>())},
		{"<", OperatorIdentity(builtin_func(lwc::is_lessthan), 0, -1, true, new TypeImpl<NumVar>())},
		{"+=", OperatorIdentity(builtin_func(lwc::incrementby), 0, -2, true, new TypeImpl<NumVar>())}
	};

	struct BuiltInIdentity {
		lwc::builtin_func fnc;
		int arg_count = 1;
		bool rval = false;
		RegisterType* rt;
		BuiltInIdentity(lwc::builtin_func _fnc, RegisterType* _rt, int _arg_count = 1, bool _rval = false ) : fnc(_fnc), arg_count(_arg_count), rval(_rval), rt(_rt) {}
		BuiltInIdentity(){};
		~BuiltInIdentity() { };
	};

	std::unordered_map<std::string, BuiltInIdentity> func_ids = {
		{"while", BuiltInIdentity(builtin_func(lwc::while_loop), new TypeImpl<NumVar>())},
		{"print", BuiltInIdentity(builtin_func(lwc::print), new TypeImpl<NumVar>())}
	};

	bool is_num(const std::string& s)
	{
		for (char c : s) { if (!isdigit(c)) return false; }
	}

	enum class TokenType { num, op, name, lparen, rparen, func, comma, elastic };

	struct LAST;

	struct LAST_Variable;

	struct LineNode;

	lwc::variable evaluate_line(lwc::LineNode* node);

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

		ParseToken(std::string _val, TokenType _tt, RegisterType* _rt, int _precedence = 0, bool _leftassoc = 0, bool _rval = false) : val(_val), tt(_tt), rt(_rt), precedence(_precedence), leftassoc(_leftassoc), rval(_rval) {
			if (tt == TokenType::func && func_ids.count(val)) {
				opfunc = func_ids[val].fnc;
				argn = func_ids[val].arg_count;
				rval = func_ids[val].rval;
			}
			else if (tt == TokenType::op) {
				rval = op_ids.at(val).rval;
			}
		}
		ParseToken(OperatorIdentity oid) : tt(TokenType::op), precedence(oid.precedence), leftassoc(oid.leftassoc), opfunc(oid.fnc), rval(oid.rval), rt(oid.rt)
		{ }; //No value is possible here because the string value is irrelevant to an op
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
		
		void add_unknown(std::string &unk, QState &qs) {
			//std::cout << "unk: " << unk << endl;
			if (unk.length() > 0) {
				if (qs == QState::num)
					data.push_back(ParseToken(unk, TokenType::num, nullptr));
				else
					data.push_back(ParseToken(unk, TokenType::name, nullptr));
				qs = QState::def;
				unk.clear();
			}
		}

		bool checkparens(std::string &tmp, char &c, QState &qs) {
			bool ret = false;
			if (c == ')')
			{
				add_unknown(tmp, qs);
				data.emplace_back(std::string(1, c), TokenType::rparen, nullptr);
				ret = true;
			}
			else if (c == '(') {
				if (!tmp.empty()) {
					data.push_back(ParseToken(tmp, TokenType::func, func_ids[tmp].rt));
					tmp.clear();
				}
				data.emplace_back(std::string(1, c), TokenType::lparen, nullptr);
				ret = true;
			}
			if (ret) qs = QState::def;
			return ret;
		}

	public:
		bool brace_end = false;
		TokenQueue(std::string s) {
			std::string temp = "";
			QState qs = QState::def;
			for (char c : s) {
				if (qs == QState::elastic) {
					if (c == '`') {
						data.push_back(ParseToken(temp, TokenType::elastic, new TypeImpl<NumVar>));
						qs = QState::def;
						temp.clear();
					}
					else {
						temp += c;
					}
				}
				else if (isdigit(c)) {
					if (temp.size() > 0) {
						if (op_ids.count(temp) > 0) {
							data.emplace_back(op_ids.at(temp));
							temp = c;
							qs = QState::num;
						}
						else {
							temp += c;
						}
					}
					else {
						temp += c;
						qs = QState::num;
					}
				}
				else if (op_ids.count(temp) && !op_ids.count(temp + c)) {
					data.emplace_back(op_ids.at(temp));
					temp.clear();
					if (!checkparens(temp, c, qs)) {
						temp = c;
						qs = QState::def;
					}
				}
				else if (op_ids.count(std::string(1, c)) && !op_ids.count(temp+c)) {
					add_unknown(temp, qs);
					temp = c;
					qs = QState::op;
				}
				
				else if (c == ',') {
					add_unknown(temp, qs);
					data.emplace_back(",", TokenType::comma, nullptr);
					qs = QState::def;
				}
				else if (c == '`') {
					add_unknown(temp, qs);
					qs = QState::elastic;
				}
				else if (c == ' ') {
					continue;
				}
				else if (c == '{') {
					if (!data.empty() && data.back().tt == TokenType::rparen)
						data.back().brace_start = true;
				}
				else if (c == '}') {
					if (temp.size() > 0) {
						add_unknown(temp, qs);
					}
					brace_end = true;
					qs = QState::def;
				}
				else
				{
					if(!checkparens(temp, c, qs))
					temp += c;
				}
			}
			add_unknown(temp, qs);
		}

		ParseToken pop()
		{
			ParseToken temp = data.front();
			data.pop_front();
			return temp;
		}

		bool empty() { return data.empty(); }
	};

	std::queue<ParseToken> shunting_yard(TokenQueue tq) //Adapted Shunting-Yard algorithm. output is the output queue
	{
		std::queue<ParseToken> out_q; //output queue
		std::stack<ParseToken> op_stk; //operator stack

		while (!tq.empty()) {
			ParseToken pt = tq.pop();
			switch (pt.tt) {

			case TokenType::num:
				out_q.push(pt);
				break;
			case TokenType::name:
				out_q.push(pt);
				break;
			case TokenType::elastic:
				out_q.push(pt);
				break;
			case TokenType::func:
				op_stk.push(pt);
				break;
			case TokenType::op:
				while (!op_stk.empty() && ((op_stk.top().tt == TokenType::op) &&
					(op_stk.top() > pt || (op_stk.top().precedence == pt.precedence && pt.leftassoc)))) {
					out_q.push(op_stk.top());
					op_stk.pop();
				}
				op_stk.push(pt);
				break;
			case TokenType::comma:
				while (op_stk.top().tt != TokenType::lparen) {
					out_q.push(op_stk.top());
					op_stk.pop();
				}
				break;
			case TokenType::lparen:
				op_stk.push(pt);
				break;

			case TokenType::rparen:
				while (op_stk.top().tt != TokenType::lparen) {
					out_q.push(op_stk.top());
					op_stk.pop();
				}
				if (op_stk.top().tt == TokenType::lparen) {
					op_stk.pop();
				}
				if (!op_stk.empty()  && op_stk.top().tt == TokenType::func) {
					op_stk.top().brace_start = pt.brace_start;
					out_q.push(op_stk.top());
					op_stk.pop();
				}
			}
		}
		if (!op_stk.empty()) {
			while (!tq.empty()) {
				out_q.push(tq.pop());
			}
		}
		if (tq.empty()) {
			while (!op_stk.empty()) {
				out_q.push(op_stk.top());
				op_stk.pop();
			}
		}
		return out_q;
	}


	class LineNode {
		std::vector<LineNode*> branches;
	public:
		builtin_func func;
		bool rval = false;
		variable var = variable();
		bool is_leaf = false;
		std::vector<LAST> output_block;
		variable* arg_arr = nullptr;
		int sz = 0;
		RegisterType* rt;
		variable rgstr;
		void fit_args() {
			delete[] arg_arr;
			branches.shrink_to_fit();
			sz = branches.size();
			arg_arr = new variable[sz];
		}

		void fit_register() {
			BaseVariable* tempbv = (BaseVariable*)rt->allocate();
			rgstr = variable(tempbv);
		}

		LineNode(builtin_func _func, RegisterType* _rt, std::vector<LineNode*> _branches = {}, bool rval = false) : func(_func), rt(_rt), branches(_branches), rval(rval)
		{ 
			fit_args();
			fit_register();
		}

		LineNode(variable _var, RegisterType* _rt) : var(_var), rt(_rt) { is_leaf = true; }
		LineNode() { is_leaf = true; }
		~LineNode() { delete[] arg_arr; }

		void add_branch(LineNode* ln) {
			branches.push_back(ln);
			fit_args();
			fit_register();
		}

		LineNode* get_branch(const int& index) {
			return branches[index];
		}
	};

	class LASTVariable : public BaseVariable //variable wrapping a vector of type LAST
	{
		std::shared_ptr<LineNode> linenode;
	public:
		LASTVariable(LineNode* _linenode) : linenode(_linenode) {}
		long get()
		{
			//std::cout << "CALLING IN" << a << std::endl;
			variable v = evaluate_line(linenode.get());
			variable v2 = std::static_pointer_cast<BaseVariable>(linenode->rgstr);
			return v->get();
		}
	};

	lwc::variable convert_symbol(const ParseToken &pt, std::unordered_map<std::string, lwc::variable>& varmap) {
		if (pt.tt == TokenType::num) {
			return std::make_shared<NumVar>(long(stol(pt.val)));
		}
		else {
			if (varmap.count(pt.val) == 0) {
				lwc::n_variable temp = std::make_shared<NumVar>(long(0));
				varmap[pt.val] = temp;
				return temp;
			}
			else {
				return varmap[pt.val];
			}
		}
	}

	std::unordered_map<std::string, lwc::variable> global; //HACK: eventually pass through global scope
	struct LAST { //"Line" abstract syntax tree
		LineNode* root;
		uint8_t block_ends = 0;
		uint8_t block_starts = 0;
		LineNode* block_node = nullptr;
		LAST(std::queue<ParseToken> tq, std::unordered_map<std::string, lwc::variable>& global) { //Turn a Shunting-Yard output queue into a tree of tokens. This is needed to actually evaluate the expression
			std::stack<LineNode*> pds; //any nodes not yet childed to an operator are pushed here
			while (!tq.empty()) {
				ParseToken pt = ParseToken(tq.front());
				tq.pop();
				if (pt.tt == TokenType::op) { //When we find an operator we must pop n tokens off of pds. n=amount of operands required by given operator or function
					std::vector<LineNode*> temp;
					for (int i = 0; i < 2; ++i) { //Operators always consume two tokens
						temp.push_back(pds.top());
						pds.pop();
					}
					std::reverse(std::begin(temp), std::end(temp)); //vector must be reversed in order for the variables to be in the 'right' order
					pds.push(new LineNode(pt.opfunc, pt.rt, temp, pt.rval)); //create and push operator node with operand children
				}
				else if (pt.tt == TokenType::func) { //Function handling code is currently irrelevant as function declaration is not yet implemented
					std::vector<LineNode*> temp;
					for (int i = 0; i < pt.argn; ++i) { //TODO: let functions work for proper number of parameters
						temp.push_back(pds.top());
						pds.pop();
					}
					std::reverse(std::begin(temp), std::end(temp)); //vector must be reversed in order for the variables to be in the 'right' order
					LineNode* fln = new LineNode(pt.opfunc, pt.rt, temp, pt.rval);
					if (pt.brace_start) {
						block_node = fln;
						block_starts += 1;
					}
					
					pds.push(fln); //create and push operator node with operand children
				}
				else if (pt.tt == TokenType::elastic) {
					LAST l = LAST(shunting_yard(TokenQueue(pt.val)), global);
					pds.push(new LineNode(std::make_shared<LASTVariable>(l.root), pt.rt));
				}
				else {
					//std::cout << "whats up" << pt.val << " " <<convert_symbol(pt, global)->get() << " " << is_num(pt.val) << std::endl;
					pds.push(new LineNode(convert_symbol(pt, global), pt.rt)); //if not an operator, push to pds				
				}
			}
			if (!pds.empty()) {
				root = pds.top(); //remaining node is the root
			}
		}

		~LAST() {
			//delete root;
		}
	};


	lwc::variable evaluate_line(lwc::LineNode * const node) {

		for (int i = 0; i < node->sz ; ++i) {
			if (node->get_branch(i)->is_leaf) {
				node->arg_arr[i] = node->get_branch(i)->var;
			}
			else {
				node->arg_arr[i] = evaluate_line(node->get_branch(i));
			}
		}
		return node->func(node->arg_arr, node->rgstr, node->sz);
	}

	lwc::variable evaluate_lines(vector<lwc::LAST> &lines) {
		lwc::variable var;
		for (int i = 0; i < lines.size()-1;++i) {
			var = evaluate_line(lines[i].root);
		}
		return evaluate_line(lines[lines.size() - 1].root);
	}

	typedef std::vector<LAST> block_func;

	class CodeBlockVariable : public BaseVariable //variable wrapping a vector of type LAST
	{
		block_func code_block;
	public:
		CodeBlockVariable(block_func &_code_block) : code_block(_code_block) {}
		long get()
		{
			return evaluate_lines(code_block)->get();
		}
	};


	
	block_func parse_from_slines(std::vector<std::string> slines) {
		std::stack<block_func> blockstack;
		std::stack<LineNode*> bnodes;
		block_func main_scope;
		blockstack.push(main_scope);
		for (std::string sline : slines) {
			TokenQueue tq(sline);
			LineNode* bnode = nullptr;
			if (!tq.empty()) {
				LAST temp_last(shunting_yard(tq), global);
				blockstack.top().push_back(temp_last);

				bnode = temp_last.block_node;
			}
			if (bnode) {
				blockstack.emplace();
				bnodes.push(bnode);
			}
			else if (tq.brace_end) {
				std::shared_ptr<CodeBlockVariable> cbv = std::make_shared<CodeBlockVariable>(blockstack.top());
				blockstack.pop();
				LineNode *ln = new LineNode(cbv, new TypeImpl<NumVar>);
				bnodes.top()->add_branch(ln);
				bnodes.pop();
			}
		}
		return blockstack.top();
	}


}



#endif