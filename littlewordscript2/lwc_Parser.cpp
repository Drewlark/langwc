#include "lwc_Parser.h"

namespace lwc {

	bool is_num(const std::string& s)
	{
		for (char c : s) { if (!isdigit(c)) return false; }
	}

	LAST::LAST(std::queue<ParseToken> tq, std::unordered_map<std::string, lwc::variable>& global) { //Turn a Shunting-Yard output queue into a tree of tokens. This is needed to actually evaluate the expression
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
				for (int i = 0; i < pt.argn; ++i) {
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

	void LineNode::fit_args() {
		delete[] arg_arr;
		branches.shrink_to_fit();
		sz = branches.size();
		arg_arr = new variable[sz];
	}

	void LineNode::fit_register() {
		BaseVariable* tempbv = (BaseVariable*)rt->allocate();
		rgstr = variable(tempbv);
	}

	LineNode::LineNode(builtin_func _func, RegisterType* _rt, std::vector<LineNode*> _branches, bool rval) : func(_func), rt(_rt), branches(_branches), rval(rval)
	{
		fit_args();
		fit_register();
	}

	void LineNode::add_branch(LineNode* ln) {
		branches.push_back(ln);
		fit_args();
	}

	LineNode* LineNode::get_branch(const int& index) {
		return branches[index];
	}


	std::unordered_map<std::string, lwc::variable> global; //HACK: eventually pass through global scope

	const static TypeImpl<LASTVariable> LAST_TYPEI;
	const static TypeImpl<CodeBlockVariable> CB_TYPEI;

	const RegisterType const* CodeBlockVariable::typei = &CB_TYPEI;
	const RegisterType const* LASTVariable::typei = &LAST_TYPEI;


	long CodeBlockVariable::get()
	{
		return evaluate_lines(code_block)->get();
	}
	RegisterType const* const CodeBlockVariable::get_typei() { return typei; }

	long LASTVariable::get()
	{
		variable v = evaluate_line(linenode.get());
		return v->get();
	}
	RegisterType const* const LASTVariable::get_typei() { return typei; }

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
				if (!op_stk.empty() && op_stk.top().tt == TokenType::func) {
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

	lwc::variable convert_symbol(const ParseToken& pt, std::unordered_map<std::string, lwc::variable>& varmap) {
		if (pt.tt == TokenType::num) {
			return std::make_shared<NumVar>(long(stol(pt.val)));
		}
		else {
			if (varmap.count(pt.val) == 0) {
				lwc::variable temp = std::make_shared<NumVar>(long(0));
				varmap[pt.val] = temp;
				return temp;
			}
			else {
				return varmap[pt.val];
			}
		}
	}

	lwc::variable evaluate_line(lwc::LineNode* const node) {

		for (int i = 0; i < node->sz; ++i) {
			if (node->get_branch(i)->is_leaf) {
				node->arg_arr[i] = node->get_branch(i)->var;
			}
			else {
				node->arg_arr[i] = evaluate_line(node->get_branch(i));
			}
		}
		return node->func(node->arg_arr, node->rgstr, node->sz);
	}

	lwc::variable evaluate_lines(block_func lines) {
		lwc::variable var;
		for (LAST line : lines) {
			var = evaluate_line(line.root);
			std::cout << var->get() << std::endl;
		}
		return var;
	}

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
				LineNode* ln = new LineNode(cbv, new TypeImpl<NumVar>);
				bnodes.top()->add_branch(ln);
				bnodes.pop();
			}
		}
		return blockstack.top();
	}

}