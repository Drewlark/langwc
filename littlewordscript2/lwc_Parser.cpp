#include "lwc_Parser.h"
#include "lwc_UserFuncs.h"
#include <algorithm>
#ifdef _DEBUG
#include <functional>
std::vector<std::reference_wrapper<lwc::LineNode>> debug_set;
#endif

namespace lwc {

	bool is_num(const std::string& s)
	{
		for (char c : s) { if (!isdigit(c)) return false; }
	}

	void LineNode::fit_args() {
		delete[] arg_arr;
		branches.shrink_to_fit();
		sz = branches.size();
		arg_arr = new variable * [sz];
	}

	void LineNode::fit_register() {
		//if (!this->is_leaf) {
		BaseVariable* tempbv = (BaseVariable*)rt->allocate();
		rgstr = variable(tempbv);
		//}
	}

	void LineNode::fit_leaf_states()
	{
		leaf_states.clear();
		for (unsigned int i = 0; i < sz; ++i) {
			leaf_states.push_back(get_branch(i)->is_leaf);
		}
	}

	inline bool LineNode::get_leaf_state(const int& i)
	{
		return leaf_states[i];
	}

	LineNode::LineNode(TokenType _tt, std::shared_ptr<master_lns> _master, builtin_func _func,
		const RegisterType* _rt, branches_t _branches, bool rval) 
		: func(_func), rt(_rt), branches(_branches), is_rval(rval), master(_master), tt(_tt)
	{
		fit_args();
		fit_register();
		fit_leaf_states();
#ifdef _DEBUG
		debug_set.push_back(*this);
		;;
#endif

	}

	void LineNode::add_branch(LineNode* ln) {
		master->push_back(ln);
		branches.push_back(master->size() - 1);
		fit_args();
		fit_leaf_states();
	}

	LineNode* LineNode::get_branch(const int& index) {
		// TODO rather than access branches from master at run-time, allow for initialization step that simply creates vector of branches directly to linenodes. Would also need custom copy constructor
		return (*master)[branches[index]];
	}

	const static TypeImpl<LASTVariable> LAST_TYPEI;
	const static TypeImpl<CodeBlockVariable> CB_TYPEI;

	const RegisterType* CodeBlockVariable::typei = &CB_TYPEI;
	const RegisterType* LASTVariable::typei = &LAST_TYPEI;


	long CodeBlockVariable::get()
	{
		return evaluate_lines(code_block)->get();
	}
	RegisterType const* const CodeBlockVariable::get_typei() { return typei; }

	long LASTVariable::get()
	{
		variable v = evaluate_line(*linenode.get());
		return v->get();
	}
	RegisterType const* const LASTVariable::get_typei() { return typei; }

	// Adapted Shunting-Yard algorithm. output is the output queue
	TokenQueue shunting_yard(TokenQueue tq) 
	{
		
		TokenQueue out_q(tq.brace_starts, tq.brace_ends); // output queue
		std::stack<ParseToken> op_stk; // operator stack

		while (!tq.empty()) {
			ParseToken pt = tq.pop();
			switch (pt.tt) {

			case TokenType::NUM:
				out_q.push(pt);
				break;
			case TokenType::NAME:
				out_q.push(pt);
				break;
			case TokenType::ELASTIC:
				out_q.push(pt);
				break;
			case TokenType::CALL:
				op_stk.push(pt);
				break;
			case TokenType::OP:
				while (!op_stk.empty() && ((op_stk.top().tt == TokenType::OP) &&
					(op_stk.top() > pt || 
					(op_stk.top().precedence == pt.precedence && pt.leftassoc)))) 
				{
					out_q.push(op_stk.top());
					op_stk.pop();
				}
				op_stk.push(pt);
				break;
			case TokenType::COMMA:
				while (op_stk.top().tt != TokenType::LPAREN) {
					out_q.push(op_stk.top());
					op_stk.pop();
				}
				break;
			case TokenType::LPAREN:
				op_stk.push(pt);
				break;

			case TokenType::RPAREN:
				while (op_stk.top().tt != TokenType::LPAREN) {
					out_q.push(op_stk.top());
					op_stk.pop();
				}
				if (op_stk.top().tt == TokenType::LPAREN) {
					op_stk.pop();
				}
				if (!op_stk.empty() && op_stk.top().tt == TokenType::CALL) {
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
#ifdef _DEBUG
		auto newq = out_q;
		while (!newq.empty()) {
			std::cout << newq.front().val << " ";
			newq.pop();
		}
		std::cout << std::endl;
#endif
		return out_q;
	}

	lwc::variable convert_symbol(const ParseToken& pt, Scope& scope) {
		if (pt.tt == TokenType::NUM) {
			return new NumVar(long(stol(pt.val)));
		}
		else {
			scope.handle_generic_token(pt);
			return nullptr;
		}
	}

	LAST::LAST(TokenQueue tq, Scope& scope) {
		// Turn a Shunting-Yard output queue into a tree of tokens. This is needed to actually evaluate the expression
		std::stack<LineNode*> pds; //any nodes not yet childed to an operator are pushed here
		while (!tq.empty()) {
			ParseToken pt(tq.front());
			tq.void_pop();

			switch (pt.tt) {
			case (TokenType::OP):
			{
				branches_t temp;
				for (int i = 0; i < 2; ++i) {
					master->push_back(pds.top());
					temp.push_back(master->size() - 1);
					pds.pop();
				}
				std::reverse(std::begin(temp), std::end(temp));
				pds.push(new LineNode(pt.tt, master, pt.opfunc, pt.rt, temp, pt.rval));
				break;
			}
			case (TokenType::CALL):
			{
				branches_t temp;
				for (int i = 0; i < pt.argn; ++i) {
					master->push_back(pds.top());
					temp.push_back(master->size() - 1);
					pds.pop();

				}
				std::reverse(std::begin(temp), std::end(temp));
				LineNode* fln = new LineNode(pt.tt, master, pt.opfunc, pt.rt, temp, pt.rval);
				if (pt.brace_start) {
					block_node = fln;
					block_starts += 1;
				}

				pds.push(fln);
				break;
			}
			case(TokenType::ELASTIC):
			{
				LAST l = LAST(shunting_yard(TokenQueue(pt.val)), scope);
				LASTVariable* lvp = new LASTVariable(l.root);
				pds.push(new LineNode(pt.tt, lvp, pt.rt));
				break;
			}
			case(TokenType::DECL_FUNC):
			{
				branches_t temp;
				for (int i = 0; i < pt.argn; ++i) {
					master->push_back(pds.top());
					temp.push_back(master->size() - 1);
					pds.pop();

				}
				std::reverse(std::begin(temp), std::end(temp));
				LineNode* fln = new LineNode(pt.tt, master, pt.opfunc, pt.rt, temp, pt.rval);
				if (pt.brace_start) {
					block_node = fln;
					block_starts += 1;
				}
				flags.insert(LASTFlags::DECL);
				pds.push(fln);
				break;
			}
			default:
				variable v = convert_symbol(pt, scope);

				// lvals are references into scope object
				v ? pds.push(new LineNode(pt.tt, v, pt.rt)) : pds.push(new LineNode(pt.tt, scope[pt.val], pt.rt, pt.val));
				breadthwise.push_back(pds.top());
			}
		}
		if (!pds.empty()) {
			root = pds.top(); //remaining node is the root
		}
#ifdef PRINT_LVAL_INFO
		print_lval_info();
#endif // _DEBUG
	}

	/*LAST::LAST(const LAST& last2)
	{
		for (auto lnp : *last2.master) {
			master->emplace_back(new LineNode(lnp))
		}
	}*/


	// Instead of using hashing every function call, scope could return two ids which
	//		would point to pointers on an array held by Scope
	void LAST::print_lval_info() {
		for (LineNode*& lnp : breadthwise) {
			//std::cout << "bw" << lnp->is_leaf << std::endl;
			if (!(lnp->is_rval)) {
				std::cout << "LVAL_INFO " << sizeof(lnp) << " " << lnp->name << " " << lnp->data.lvar << std::endl;
			}
		}
	}

	lwc::variable& evaluate_line(lwc::LineNode& node) {

		for (unsigned int i = 0; i < node.sz; ++i) {
			if (node.get_leaf_state(i)) {
				node.arg_arr[i] = node.get_branch(i)->get_leaf();
			}
			else {
				node.arg_arr[i] = &evaluate_line(*node.get_branch(i));
			}
		}
		return node.func(node.arg_arr, node.rgstr, node.sz);
	}

	lwc::variable& evaluate_lines(CodeBlock& lines) {
		lwc::variable* var = nullptr;
		for (int i = 0; i < lines.size(); ++i) {
			var = &evaluate_line(*lines[i].root);
		}
		return *var;
	}

	CodeBlock r_parse_tq(const std::vector<TokenQueue> &lines_vec, Scope& scope, int starts = 0) {
		CodeBlock cb;
		std::vector<TokenQueue> tq_buffer;
		for (auto line : lines_vec) {
			if (starts <= 0) {
				// std::cout << (int)line.brace_starts << '\n';
				if(!line.empty()) cb.emplace_back(line, scope);
			}
			else {
				tq_buffer.push_back(line);
				starts -= line.brace_ends;
				// std::cout << (int)line.brace_ends << '\n';
				if (starts <= 0) {
					auto cbv = new CodeBlockVariable(r_parse_tq(tq_buffer, scope));
					LineNode* ln = new LineNode(TokenType::NUM, cbv, &NUM_TYPEI);
					cb.back().block_node->add_branch(ln);
					starts = 0;
				}
			}
			starts += line.brace_starts;
		}
		return cb;
	}

	CodeBlock _parse_tq(std::vector<TokenQueue> lines_vec, Scope& scope)
	{
		std::stack<CodeBlock> blockstack;
		std::stack<LineNode*> bnodes;
		CodeBlock main_scope;
		blockstack.push(main_scope);

		for (auto line : lines_vec) {
			LineNode* bnode = nullptr;
			if (!line.empty()) {

				blockstack.top().emplace_back(line, scope);
				bnode = blockstack.top().back().block_node; // block_node here might also be nullptr
			}

			if (bnode) { // bnode will only be set if this line's bnode was not nullptr
				blockstack.emplace();
				bnodes.push(bnode);
			}
			else if (line.brace_ends) {
				CodeBlockVariable* cbv = new CodeBlockVariable(blockstack.top());
				blockstack.pop();
				LineNode* ln = new LineNode(TokenType::NUM, cbv, &NUM_TYPEI);
				bnodes.top()->add_branch(ln);
				bnodes.pop();
			}
		}
		return blockstack.top();
	}

	// parse tokenqueue that has been preprocessed with TokenQueue and shunting_yard
	CodeBlock threadsafe_parse_from_tq(std::vector<TokenQueue> lines_vec, Scope scope) {
		return _parse_tq(lines_vec, scope);
	}

	std::vector<TokenQueue> preprocess_slines(const std::vector<std::string>& slines) {
		std::vector<TokenQueue> lines;
		for (const auto& line : slines) {
			lines.emplace_back(shunting_yard(TokenQueue(line)));
		}
	}

	struct ParsedBlock {
		TokenQueue header;
		std::vector<TokenQueue> body;
		ParsedBlock(const LinkerData &ld, const std::vector<std::string>& slines) 
		: header(slines[ld.start]){
			auto first = slines.begin() + ld.start+1;
			auto last = slines.begin() + ld.end+1;
			std::vector<std::string> sublines(first, last);
			body = preprocess_slines(sublines);
		}
	};

	CodeBlock parse_from_slines(std::vector<std::string> slines, Scope& scope) {
		std::vector<TokenQueue> lines_vec;
		std::unordered_map<unsigned int, ParsedBlock> pblockmap;
		for (const auto& block : block_map) {
			pblockmap.emplace(block.start, ParsedBlock(block, slines));
		}
		for (auto& line : slines) {
			TokenQueue tq(line);
			lines_vec.push_back(shunting_yard(tq));
		}
		return r_parse_tq(lines_vec, scope);
	}



}