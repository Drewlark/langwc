#include <unordered_map>
#include <string>
#ifndef H_LWC_LEXER
#define H_LWC_LEXER

namespace lwc{
	static enum class Keywords {INT, ELAST, FUNC};
	static const std::unordered_map<std::string, Keywords> KEYWORD_STRINGS = {
		{"int", Keywords::INT}, {"elast", Keywords::ELAST}, {"func", Keywords::FUNC}
	};
}




#endif