#include <iostream>
#include <string>
#include <set>
#include "lwc_typedefs.h"
#include "lwc_builtins.h"
#ifndef H_PARSER
#define H_PARSER
namespace lwc {

	static const set<char> reserved_chars = { '+', '*', '-', '/', '=', '<', '>', '$', ';' };

	static const std::unordered_map<std::string, OperatorIdentity> op_ids = {
		{"+", OperatorIdentity(builtin_func(lwc::add), false, 0, true, new TypeImpl<NumVar>(), "ADD")}, // HACK temporary solution to statically define return types.
		{"*", OperatorIdentity(builtin_func(lwc::mult), false , 1, true, new TypeImpl<NumVar>(), "MULT")},
		{"-", OperatorIdentity(builtin_func(lwc::sub), true, 0, true, new TypeImpl<NumVar>(), "SUB")},
		{"/", OperatorIdentity(builtin_func(lwc::div), true, 1, true, new TypeImpl<NumVar>(), "DIV")},
		{"=", OperatorIdentity(builtin_func(lwc::assign), false , -2, true, new TypeImpl<NumVar>(), "ASGN")},
		{"<", OperatorIdentity(builtin_func(lwc::is_lessthan), false, -1, true, new TypeImpl<NumVar>(), "LSSR")},
		{">", OperatorIdentity(builtin_func(lwc::is_greaterthan), false, -1, true, new TypeImpl<NumVar>(), "GRTR")},
		{"+=", OperatorIdentity(builtin_func(lwc::incrementby), false, -2, true, new TypeImpl<NumVar>(), "INCR")},
		{"=>", OperatorIdentity(builtin_func(lwc::refassign), false , -2, true, new TypeImpl<NumVar>(), "RAS")},
		{"$", OperatorIdentity(builtin_func(lwc::bad_name), false, -2, true, new TypeImpl<NumVar>(), "BN")},
		{"=$", OperatorIdentity(builtin_func(lwc::it_elast), false, -2, true, new TypeImpl<LASTVariable>, "I_E")},
		{";", OperatorIdentity(builtin_func(lwc::nihil), false, -99, true, new TypeImpl<NumVar>(), "NHL")},
	};

	static std::unordered_map<std::string, BuiltInIdentity> func_ids = {
		{"while", BuiltInIdentity(builtin_func(lwc::while_loop), new TypeImpl<NumVar>())},
		{"print", BuiltInIdentity(builtin_func(lwc::print), new TypeImpl<NumVar>())},
		{"scast", BuiltInIdentity(builtin_func(lwc::scast), new TypeImpl<NumVar>())},
		{"pxnor", BuiltInIdentity(builtin_func(lwc::sexecute), new TypeImpl<NumVar>())}
	};

	bool is_num(const std::string& s);

	//lwc::variable evaluate_line(lwc::LineNode* node);

	TokenQueue shunting_yard(TokenQueue tq); //Adapted Shunting-Yard algorithm. returns the output queue

	lwc::variable convert_symbol(const ParseToken& pt, Scope& scope);

	lwc::variable &evaluate_line(lwc::LineNode& node);

	lwc::variable &evaluate_lines(CodeBlock &lines);

	CodeBlock parse_from_slines(std::vector<std::string> slines);

	CodeBlock parse_from_tq(std::vector<TokenQueue>);
}
#endif