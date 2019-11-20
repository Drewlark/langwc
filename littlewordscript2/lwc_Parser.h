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
		{"+", OperatorIdentity(builtin_func(lwc::add), false, 0, true, &NUM_TYPEI, "ADD")}, // HACK temporary solution to statically define return types.
		{"*", OperatorIdentity(builtin_func(lwc::mult), false , 1, true, &NUM_TYPEI, "MULT")},
		{"-", OperatorIdentity(builtin_func(lwc::sub), true, 0, true, &NUM_TYPEI, "SUB")},
		{"/", OperatorIdentity(builtin_func(lwc::div), true, 1, true, &NUM_TYPEI, "DIV")},
		{"=", OperatorIdentity(builtin_func(lwc::assign), false , -2, true, &NUM_TYPEI, "ASGN")},
		{"<", OperatorIdentity(builtin_func(lwc::is_lessthan), false, -1, true, &NUM_TYPEI, "LSSR")},
		{">", OperatorIdentity(builtin_func(lwc::is_greaterthan), false, -1, true, &NUM_TYPEI, "GRTR")},
		{"+=", OperatorIdentity(builtin_func(lwc::incrementby), false, -2, true, &NUM_TYPEI, "INCR")},
		{"=>", OperatorIdentity(builtin_func(lwc::refassign), false , -2, true, &NUM_TYPEI, "RAS")},
		{"$", OperatorIdentity(builtin_func(lwc::bad_name), false, -2, true, &NUM_TYPEI, "BN")},
		{"=$", OperatorIdentity(builtin_func(lwc::it_elast), false, -2, true, new TypeImpl<LASTVariable>, "ELAS")},
		{";", OperatorIdentity(builtin_func(lwc::nihil), false, -99, true, &NUM_TYPEI, "NHL")},
	};

	static std::unordered_map<std::string, BuiltInIdentity> func_ids = {
		{"while", BuiltInIdentity(builtin_func(lwc::while_loop), &NUM_TYPEI)},
		{"print", BuiltInIdentity(builtin_func(lwc::print), &NUM_TYPEI)},
		{"scast", BuiltInIdentity(builtin_func(lwc::scast), &NUM_TYPEI)},
		{"pxnor", BuiltInIdentity(builtin_func(lwc::sexecute), &NUM_TYPEI)}
	};

	

	bool is_num(const std::string& s);

	//lwc::variable evaluate_line(lwc::LineNode* node);

	TokenQueue shunting_yard(TokenQueue tq); //Adapted Shunting-Yard algorithm. returns the output queue

	lwc::variable convert_symbol(const ParseToken& pt, Scope& scope);

	lwc::variable &evaluate_line(lwc::LineNode& node);

	lwc::variable &evaluate_lines(CodeBlock &lines);

	CodeBlock _parse_tq(std::vector<TokenQueue> lines_vec, Scope &scope);

	CodeBlock parse_from_slines(std::vector<std::string> slines, Scope &scope);

	CodeBlock threadsafe_parse_from_tq(std::vector<TokenQueue>, Scope scope);
}
#endif