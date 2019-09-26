#include "lwc_typedefs.h"
#include "lwc_Parser.h"
namespace lwc {
	
	ParseToken::ParseToken(std::string _val, TokenType _tt, RegisterType* _rt, int _precedence, bool _leftassoc, bool _rval) : val(_val), tt(_tt), rt(_rt), precedence(_precedence), leftassoc(_leftassoc), rval(_rval) {
		if (tt == TokenType::func && func_ids.count(val)) {
			opfunc = func_ids[val].fnc;
			argn = func_ids[val].arg_count;
			rval = func_ids[val].rval;
		}
		else if (tt == TokenType::op) {
			rval = op_ids.at(val).rval;
		}
	}
	
	void TokenQueue::check_for_argness() {
		if (!func_stack.empty()) {
			++func_stack.top()->argn;
		}
	}

	void TokenQueue::add_unknown(std::string& unk, QState& qs) {
		//std::cout << "unk: " << unk << endl;
		if (unk.length() > 0) {
			if (qs == QState::num)
				data.push_back(ParseToken(unk, TokenType::num, nullptr));
			else
				data.push_back(ParseToken(unk, TokenType::name, nullptr));
			qs = QState::def;
			unk.clear();
			check_for_argness();
		}
	}

	bool TokenQueue::checkparens(std::string& tmp, char& c, QState& qs, bool& pcheck) {
		bool ret = false;
		if (!pcheck) {
			pcheck = true;
			if (c == ')')
			{
				add_unknown(tmp, qs);
				data.emplace_back(std::string(1, c), TokenType::rparen, nullptr);
				ret = true;
				--paren_depth;
				if (paren_depth == 0 && !func_stack.empty()) {
					func_stack.pop();
					check_for_argness();
				}
				
			}
			else if (c == '(') {
				if (!tmp.empty()) {
					data.push_back(ParseToken(tmp, TokenType::func, func_ids[tmp].rt));
					func_stack.push(&data.back());
					tmp.clear();
				}
				++paren_depth;
				data.emplace_back(std::string(1, c), TokenType::lparen, nullptr);
				ret = true;
			}
			if (ret) qs = QState::def;
		}
		return ret;
	}

	TokenQueue::TokenQueue(std::string s) {
		std::string temp = "";
		QState qs = QState::def;
		for (char c : s) {
			bool parens_checked = false;
			bool tc_appended = false;
			if (qs == QState::op) {
				if (op_ids.count(temp) && !op_ids.count(temp + c)) {
					data.emplace_back(op_ids.at(temp));
					temp.clear();
					if (!checkparens(temp, c, qs, parens_checked) && c != '`') {
						//temp = c;
						//tc_appended = true;
						qs = QState::def;
					}
				}
			}
			if (qs == QState::elastic) {
				if (c == '`') {
					data.push_back(ParseToken(temp, TokenType::elastic, new TypeImpl<NumVar>));
					qs = QState::def;
					temp.clear();
					if (!func_stack.empty()) {
						++func_stack.top()->argn;
					}
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
			else if (c == '`') {
				add_unknown(temp, qs);
				qs = QState::elastic;
			}
			else if (op_ids.count(std::string(1, c)) && !op_ids.count(temp + c)) {
				add_unknown(temp, qs);
				temp = c;
				qs = QState::op;
			}

			else if (c == ',') {
				add_unknown(temp, qs);
				data.emplace_back(",", TokenType::comma, nullptr);
				/*if (!func_stack.empty()) {
					++func_stack.top()->argn;
				}*/
				qs = QState::def;
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
				if (!checkparens(temp, c, qs, parens_checked) && !tc_appended)
					temp += c;
			}
		}
		add_unknown(temp, qs);
	}

	ParseToken TokenQueue::pop()
	{
		ParseToken temp = data.front();
		data.pop_front();
		return temp;
	}

	bool TokenQueue::empty() { return data.empty(); }
}