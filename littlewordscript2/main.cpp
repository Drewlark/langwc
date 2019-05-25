#include "lwc_builtins.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <map>
#include <string>
//Testing git push from different machine
std::vector<string> splitString(const string &s, const string &delim)
{
	std::vector<string> ret;
	std::string curr = "";
	int i;
	for(i=0; i<s.length()-(delim.length()-1); i++){
		std::string c = s.substr(i, delim.length());
		if (c == delim) {
			ret.push_back(curr);
			i += delim.length() - 1;
			curr = "";
		}
		else {
			curr += s[i];
		}
	}
	try {
		curr += s.substr(i,s.length()-i);
	}
	catch (std::out_of_range e) {
		cout << "WARNING, caught range error with : " << s << " " << delim << endl;
	}
	if (curr != "")
		ret.push_back(curr);
	return ret;
}

/*vector<string> splitString(const string &s, const string &delim)
{
	vector<string> ret;
	string curr = "";
	if (s.length() > delim.length()){
		for (int i = 0; i < s.length() - (delim.length() - 1); i++) {
			string substr_to_check = s.substr(i, delim.length());
			if (delim == substr_to_check) {
				ret.push_back(curr);
			}
			else {
				curr += s[i];
			}
		}
		for (int i = s.length() - (delim.length() - 1); i < s.length(); i++) {
			curr += s[i];
		}
	}
	else {
		ret.push_back(s);
	}
	return ret;
}*/

bool is_num(const string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}


void remove_whitespace(std::string &str) {
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

bool hasex(const std::string &line, const std::string &symbol, std::vector<std::string> &ref_vector) {
	ref_vector = splitString(line, symbol);
	if (ref_vector.size() <= 1) {
		return false;
	}
	return true;
}

lwc::variable parse_symbol(const std::string &sym, std::unordered_map<std::string, lwc::variable> &varmap) {
	if (is_num(sym)) {
		return lwc::variable(new long(stol(sym)));
	}
	else {
		if (varmap.count(sym) == 0) {
			lwc::variable temp = lwc::variable(new long(0));
			varmap[sym] = temp;
			return temp;
		}
		else {
			return varmap[sym];
		}
	}
}

std::vector<lwc::variable> lwc_get_vars(const std::vector<std::string> &vars, std::unordered_map<std::string, lwc::variable> &varmap) {
	vector<lwc::variable> realvars;
	for (std::string s : vars) {
		if(s!="")
			realvars.push_back(parse_symbol(s, varmap));
	}
	return realvars;
}

std::string remove_chars(std::string &s, const char &c) {
	s.erase(remove(s.begin(), s.end(), c), s.end());
	return s;
}


std::vector<string> seek_block(const std::vector<string> &slines, const int &start) {
	if (slines[start][slines[start].length()-1] != '{') {
		cout << "ERROR: Expected { at line " << start << endl;
		throw exception();
	}
	std::vector<std::string> lines;
	int starts = 1;
	for (int lnum = start+1; lnum < slines.size(); ++lnum) {
		const std::string &line = slines[lnum];
		if (slines[lnum][slines[lnum].length() - 1] == '}') {
			--starts;
		}
		if (starts == 0) {
			break;
		}
		else {
			lines.push_back(line);
		}
		if (slines[lnum][slines[lnum].length() - 1] == '{') {
			++starts;
		}
	}
	return lines;
}

std::vector<std::string> expand_slines(std::vector<std::string> slines) { //break string lines into multiple components
	std::vector<std::string> fin;
	for (std::string& line : slines) {

	}
}

std::vector<std::pair<std::string, lwc::builtin_func>> bin_op_table =
{
	{"+=", lwc::incrementby},
	{"-=", lwc::decrementby},
	{"+", lwc::add},
	{"-", lwc::sub},
	{":", lwc::print},
	{"=", lwc::assign}
};

std::vector<lwc::Line> build_lines(const std::vector<std::string> &slines, unordered_map<std::string, lwc::variable> varmap = {}) {
	static list<std::vector<lwc::Line>> block_heap;
	std::vector<lwc::Line> built_lines;
	for (int lnum = 0; lnum < slines.size(); ++lnum) {
		std::string line = slines[lnum];
		std::vector<std::string> broken;
		if (line.substr(0, 2) == "//" || line[0] == '}') {
			continue;
		}
		auto test = [&line, &broken](const std::string &s) {return hasex(line, s, broken); }; //lambda for hasexpression check.
		auto add_line = [&broken, &varmap, &built_lines](lwc::builtin_func f) {built_lines.push_back(lwc::Line(lwc_get_vars(broken, varmap), f)); }; //lambda to add line to built_lines
		if (line.length() >= 1 && (line[0] == '?' || line[0] == '@')) {
			std::vector<std::string> blocklines = seek_block(slines, lnum);
			bool isloop = false;
			if (line[0] == '@') {
				isloop = true;
			}
			line = line.substr(1, line.length()-2);
			if (test("==")) {
				std::vector<lwc::variable> vars = lwc_get_vars(broken, varmap);
				std::vector<lwc::Line> built_block_lines = build_lines(blocklines, varmap);
				lwc::Line newl = lwc::Line(vars, lwc::is_equal);
				block_heap.push_back(built_block_lines);
				newl.linked_lines = &(block_heap.back());
				newl.loop = isloop;
				built_lines.push_back(newl);
			}
			else if (test(">")) {
				std::vector<lwc::variable> vars = lwc_get_vars(broken, varmap);
				std::vector<lwc::Line> built_block_lines = build_lines(blocklines, varmap);
				lwc::Line newl = lwc::Line(vars, lwc::is_greaterthan);
				block_heap.push_back(built_block_lines);
				newl.linked_lines = &(block_heap.back());
				newl.loop = isloop;
				built_lines.push_back(newl);
			}
			else if (test("<")) {
				std::vector<lwc::variable> vars = lwc_get_vars(broken, varmap);
				std::vector<lwc::Line> built_block_lines = build_lines(blocklines, varmap);
				lwc::Line newl = lwc::Line(vars, lwc::is_lessthan);
				block_heap.push_back(built_block_lines);
				newl.linked_lines = &(block_heap.back());
				newl.loop = isloop;
				built_lines.push_back(newl);
			}
			lnum += blocklines.size();
			continue;
		}
		
		for (const auto &funcpr : bin_op_table) {
			if (test(funcpr.first)) {
				add_line(funcpr.second);
				break;
			}
		}
	}
	return built_lines;
}



int main()
{
	std::clock_t start = clock();
	std::string fileName = "first_test.txt";
	std::fstream fs;
	std::string s;
	std::vector<std::string> words;
	
	std::cout << "Reading " << fileName << "...." << std::endl;

	fs.open(fileName.c_str());
	std::string line;
	while (std::getline(fs, line)) {
		remove_whitespace(line);
		if(line.size() > 0)
			words.push_back(line);
	}
	fs.flush();
	std::unordered_map<std::string, lwc::variable> my_varmap;
	std::vector<lwc::Line> final_lines = build_lines(words, my_varmap);
	lwc::Evaluator my_eval = lwc::Evaluator();
	std::cout << "compilation complete" << std::endl;
	std::clock_t start_eval = clock();
	my_eval.evaluate(final_lines);
	clock_t end = clock();
	double fulltime = (double)(end - start);
	double evaltime = (double)(end - start_eval);
	std::cout << "Done Evaluating" << std::endl << std::endl;
	std::cout << "FULL RUNNING TIME: " << fulltime << std::endl;
	std::cout << "BUILDING TIME: " << fulltime - evaltime << std::endl;
	std::cout << "BUILT EVALUATION TIME: " << evaltime << std::endl;
	getchar();
	return 0;
}