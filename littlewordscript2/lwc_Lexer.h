#include <unordered_map>
#include <string>
#ifndef H_LWC_LEXER
#define H_LWC_LEXER

namespace lwc {
	enum class Keywords { INT, ELAST, FUNC };
	static const std::unordered_map<std::string, Keywords> KEYWORD_STRINGS = {
		{"int", Keywords::INT}, {"elast", Keywords::ELAST}, {"func", Keywords::FUNC}
	};

	struct LinkerData {
		std::string body;
		unsigned int start;
		unsigned int end;
	};

	static std::vector<LinkerData> block_map;

	inline LinkerData build_linktree(std::string whole, unsigned int i = 0) {
		std::string data;
		std::string temp;
		bool mode = true;
		LinkerData linkn;
		linkn.start = i;
		int depth_count = 0;
		int start_line = 0;
		int last_non_whitespace = i;
		std::cout << "\n\nRECURSE:\n";
		std::cout << whole << '\n';
		for (char& c : whole) {
			if (c == '\n') {
				++i;
			}
			else if (!isspace(c) && c != '#' && c != '{' && c != '}') {
				last_non_whitespace = i;
			}
			if (mode) {
				if (c == '{') {
					mode = false;
					++depth_count;
					start_line = last_non_whitespace;
				}
				else {
					data.push_back(c);
				}
			}
			else {
				if (c == '{') {
					++depth_count;
				}
				else if (c == '}') {
					--depth_count;
				}
				if(depth_count!=0) {
					temp.push_back(c);
				}else {
					//std::tuple<std::shared_ptr<LinkerNode>, unsigned int> tup;
					std::cout << "-----RETURN-----\n";
					std::cout << temp << '\n';
					std::cout << "-----------------\n";
					block_map.push_back(build_linktree(temp, start_line));
					mode = true;
					depth_count = 0;
					temp.clear();
					linkn.end = i;
				}
			}
		}
		linkn.body = data;
		return linkn;
	};
}




#endif