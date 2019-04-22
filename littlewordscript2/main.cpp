#include "lwc_interpreter_typedefs.h"
#include "lwc_Line.h"
#include "lwc_builtins.h"
#include <fstream>
#include<iostream>
#include <algorithm>
#include <unordered_map>
using namespace std;

vector<string> splitString(const string &s, const string &delim)
{
	vector<string> ret;
	string curr = "";
	int i;
	for(i=0; i<s.length()-(delim.length()-1); i++){
		string c = s.substr(i, delim.length());
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


void remove_whitespace(string &str) {
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

bool hasex(const string &line, const string &symbol, vector<string> &ref_vector) {
	ref_vector = splitString(line, symbol);
	if (ref_vector.size() <= 1) {
		return false;
	}
	return true;
}

variable parse_symbol(const string &sym, unordered_map<string, variable> &varmap) {
	if (is_num(sym)) {
		return variable(new int(stoi(sym)));
	}
	else {
		if (varmap.count(sym) == 0) {
			variable temp = variable(new int(0));
			varmap[sym] = temp;
			return temp;
		}
		else {
			return varmap[sym];
		}
	}
}

vector<variable> lwc_get_vars(const vector<string> &vars, unordered_map<string, variable> &varmap) {
	vector<variable> realvars;
	for (string s : vars) {
		if(s!="")
			realvars.push_back(parse_symbol(s, varmap));
	}
	return realvars;
}

vector<Line> build_lines(vector<string> slines, unordered_map<string, variable> varmap = {}) {
	vector<Line> built_lines;
	for (int lnum = 0; lnum < slines.size(); ++lnum) {
		string line = slines[lnum];
		vector<string> broken;
		if (line.substr(0, 2) == "//") {
			continue;
		}
		auto test = [&line, &broken](const string &s) {return hasex(line, s, broken); };
		auto add_line = [&broken, &varmap, &built_lines](builtin_func f) {built_lines.push_back(Line(lwc_get_vars(broken, varmap), f)); };
		if (test("+=")) {
			vector<variable> vars = lwc_get_vars(broken, varmap);
			add_line(lwc::add);
			built_lines.push_back(Line(vars[0], lwc::assign, true));
		}
		else if (test("+")) {
			add_line(lwc::add);
		}
		else if (test("-")) {
			add_line(lwc::sub);
		}
		else if (test("=")) {
			add_line(lwc::assign);
		}
		else if (test(":")) {
			add_line(lwc::print);
		}
	}
	return built_lines;
}

int evaluate(vector<Line> linevec)
{
	static int last_eval = 0;
	for (Line ln : linevec) {
		if (!ln.request_last)
			last_eval = ln.func(ln.vars);
		else
		{
			vector<variable> newvars = ln.vars;
			newvars.push_back(variable(new int(last_eval)));
			last_eval = ln.func(newvars);
		}
	}
	return 0;
}

int main()
{
	string fileName = "first_test.txt";
	fstream fs;
	string s;
	vector<string> words;

	cout << "Reading " << fileName << "...." << endl;

	fs.open(fileName.c_str());
	string line;
	while (getline(fs, line)) {
		remove_whitespace(line);
		words.push_back(line);
	}
	evaluate(build_lines(words));
	fs.flush();
	cout << "Done Reading!" << endl << endl; // let the user know we are done
	//cin >> ends;
	return 0;
}