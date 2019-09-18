#include <iostream>
#include <string>
#include "lwc_typedefs.h"
#include "lwc_builtins.h"
#ifndef H_PARSER
#define H_PARSER
namespace lwc {

	static const std::unordered_map<std::string, OperatorIdentity> op_ids = {
		{"+", OperatorIdentity(builtin_func(lwc::add), 0, 0, true, new TypeImpl<NumVar>())}, //HACK temporary solution to statically define return types.
		{"*", OperatorIdentity(builtin_func(lwc::mult), 0, 1, true, new TypeImpl<NumVar>())},
		{"-", OperatorIdentity(builtin_func(lwc::sub), 1, 0, true, new TypeImpl<NumVar>())},
		{"/", OperatorIdentity(builtin_func(lwc::div), 1, 1, true, new TypeImpl<NumVar>())},
		{"=", OperatorIdentity(builtin_func(lwc::assign), 0, -2, true, new TypeImpl<NumVar>())},
		{"<", OperatorIdentity(builtin_func(lwc::is_lessthan), 0, -1, true, new TypeImpl<NumVar>())},
		{">", OperatorIdentity(builtin_func(lwc::is_greaterthan), 0, -1, true, new TypeImpl<NumVar>())},
		{"+=", OperatorIdentity(builtin_func(lwc::incrementby), 0, -2, true, new TypeImpl<NumVar>())},
		{"=>", OperatorIdentity(builtin_func(lwc::refassign), 0, -2, true, new TypeImpl<NumVar>())}
	};

	static std::unordered_map<std::string, BuiltInIdentity> func_ids = {
		{"while", BuiltInIdentity(builtin_func(lwc::while_loop), new TypeImpl<NumVar>())},
		{"print", BuiltInIdentity(builtin_func(lwc::print), new TypeImpl<NumVar>())},
		{"ncast", BuiltInIdentity(builtin_func(lwc::scast), new TypeImpl<NumVar>())}
	};

	bool is_num(const std::string& s);

	//lwc::variable evaluate_line(lwc::LineNode* node);

	std::queue<ParseToken> shunting_yard(TokenQueue tq); //Adapted Shunting-Yard algorithm. returns the output queue

	lwc::variable convert_symbol(const ParseToken& pt, std::unordered_map<std::string, lwc::variable>& varmap);

	lwc::variable &evaluate_line(lwc::LineNode* const node);

	lwc::variable &evaluate_lines(block_func lines);

	block_func parse_from_slines(std::vector<std::string> slines);
}
#endif