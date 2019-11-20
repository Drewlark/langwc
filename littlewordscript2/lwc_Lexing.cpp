#include "lwc_typedefs.h"
#include "lwc_Parser.h"
namespace lwc {
	
	ParseToken::ParseToken(std::string _val, TokenType _tt, const RegisterType* _rt, int _precedence, bool _leftassoc, bool _rval) : val(_val), tt(_tt), rt(_rt), precedence(_precedence), leftassoc(_leftassoc), rval(_rval) {
		if (tt == TokenType::CALL && func_ids.count(val)) {
			opfunc = func_ids[val].fnc;
			argn = func_ids[val].arg_count;
			rval = func_ids[val].rval;
		}
		else if (tt == TokenType::OP) {
			rval = op_ids.at(val).rval;
		}
	}
	
	void TokenQueue::check_for_argness() {
		if (!func_stack.empty()) {
			++func_stack.top().first.argn;
		}
	}

	void TokenQueue::dump_keyword_q() {
		data.back().keywords = keyword_buffer;
	}

	void TokenQueue::add_unknown(std::string& unk, QState& qs) {
		//std::cout << "unk: " << unk << endl;
		if (!unk.empty()) {
			if (KEYWORD_STRINGS.count(unk)) {
				keyword_buffer.insert(KEYWORD_STRINGS.at(unk));
			}
			else if (qs == QState::NUM)
				data.push_back(ParseToken(unk, TokenType::NUM, nullptr));
			else {
				data.push_back(ParseToken(unk, TokenType::NAME, nullptr));
				if (!keyword_buffer.empty()) {
					dump_keyword_q();
				}
			}
			qs = QState::DEF;
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
				data.emplace_back(std::string(1, c), TokenType::RPAREN, nullptr);
				ret = true;
				--paren_depth;
				
				if (!func_stack.empty() && func_stack.top().second == paren_depth) {
					func_stack.pop();
					check_for_argness();
				}
				
			}
			else if (c == '(') {
				
				if (!tmp.empty() && !op_ids.count(tmp)) {
					keyword_buffer.count(Keywords::FUNC)  ?  // give new token proper type, declaration or call
						data.push_back(ParseToken(tmp, TokenType::DECL_FUNC, func_ids[tmp].rt)) : 
						data.push_back(ParseToken(tmp, TokenType::CALL, func_ids[tmp].rt));
					
					dump_keyword_q(); 
					// check if this is a function definition rather than just a call
					func_stack.push({ data.back(), paren_depth });
					tmp.clear();
				}
				++paren_depth;
				data.emplace_back(std::string(1, c), TokenType::LPAREN, nullptr);
				ret = true;
			}
			if (ret) qs = QState::DEF;
		}
		else {
			ret = c == '(' || c == ')';// if this function has already been called, we still want to know the nature of the character without the extra operations
		}
		return ret;
	}

	TokenQueue::TokenQueue(std::string s) {
		std::string temp = "";
		QState qs = QState::DEF;
		for (char c : s) {
			if (c == '#') {
				return;
			}
			bool parens_checked = false;
			bool tc_appended = false;
			if (qs == QState::OP) {
				if (op_ids.count(temp) && !op_ids.count(temp + c)) {
					data.emplace_back(op_ids.at(temp));
					temp.clear();
					if (!checkparens(temp, c, qs, parens_checked) && c != '`') {
						//temp = c;
						//tc_appended = true;
						qs = QState::DEF;
					}
				}
			}
			if (qs == QState::ELASTIC) {
				if (c == '`') {
					data.push_back(ParseToken(temp, TokenType::ELASTIC, &NUM_TYPEI));
					qs = QState::DEF;
					temp.clear();
					if (!func_stack.empty()) {
						++func_stack.top().first.argn;
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
						qs = QState::NUM;
					}
					else {
						temp += c;
					}
				}
				else {
					temp += c;
					qs = QState::NUM;
				}
			}
			else if (c == '`') {
				add_unknown(temp, qs);
				qs = QState::ELASTIC;
			}

			else if ((reserved_chars.count(c) && !op_ids.count(temp + c)) || reserved_chars.count(c) && temp.empty()) {
				add_unknown(temp, qs);
				temp = c;
				qs = QState::OP;
			}

			else if (c == ',') {
				add_unknown(temp, qs);
				data.emplace_back(",", TokenType::COMMA, nullptr);
				/*if (!func_stack.empty()) {
					++func_stack.top()->argn;
				}*/
				qs = QState::DEF;
			}
			else if (c == ' ') {
				if (qs == QState::DEF) {
					add_unknown(temp, qs);
				}
				continue;
			}
			else if (c == '{') {
				if (!data.empty() && data.back().tt == TokenType::RPAREN)
					data.back().brace_start = true;
				++brace_starts;
			}
			else if (c == '}') {
				if (temp.size() > 0) {
					add_unknown(temp, qs);
				}
				++brace_ends;
				qs = QState::DEF;
			}
			else
			{
				if (!checkparens(temp, c, qs, parens_checked) && !tc_appended)
					temp += c;
			}
		}
		add_unknown(temp, qs);
#ifdef _DEBUG
		auto newq = data;
		while (!newq.empty()) {
			std::cout << newq.front().val << " ";
			newq.pop_front();
		}
		std::cout << std::endl;
#endif
	}

	ParseToken TokenQueue::pop()
	{
		ParseToken temp = data.front();
		data.pop_front();
		return temp;
	}

	bool TokenQueue::empty() { return data.empty(); }
}