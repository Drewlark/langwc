#include "lwc_interpreter_typedefs.h"
#include "lwc_Line.h"
#include "lwc_builtins.h"
#include <fstream>
#include<iostream>
#include <algorithm>
#include <unordered_map>
#include <ctime>
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
		return variable(new long(stoi(sym)));
	}
	else {
		if (varmap.count(sym) == 0) {
			variable temp = variable(new long(0));
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

string remove_chars(string &s, const char &c) {
	s.erase(remove(s.begin(), s.end(), c), s.end());
	return s;
}


vector<string> seek_block(const vector<string> &slines, const int &start) {
	if (slines[start][slines[start].length()-1] != '{') {
		cout << "ERROR: Expected { at line " << start << endl;
		throw exception();
	}
	vector<string> lines;
	int starts = 1;
	for (int lnum = start+1; lnum < slines.size(); ++lnum) {
		const string &line = slines[lnum];
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

vector<Line> build_lines(const vector<string> &slines, unordered_map<string, variable> varmap = {}) {
	vector<Line> built_lines;
	for (int lnum = 0; lnum < slines.size(); ++lnum) {
		string line = slines[lnum];
		vector<string> broken;
		if (line.substr(0, 2) == "//" || line[0] == '}') {
			continue;
		}
		auto test = [&line, &broken](const string &s) {return hasex(line, s, broken); }; //lambda for hasexpression check.
		auto add_line = [&broken, &varmap, &built_lines](builtin_func f) {built_lines.push_back(Line(lwc_get_vars(broken, varmap), f)); }; //lambda to add line to built_lines
		if (line.length() >= 1 && (line[0] == '?' || line[0] == '@')) {
			vector<string> blocklines = seek_block(slines, lnum);
			bool isloop = false;
			if (line[0] == '@') {
				isloop = true;
			}
			line = line.substr(1, line.length()-2);
			if (test("==")) {
				vector<variable> vars = lwc_get_vars(broken, varmap);
				vector<Line> built_block_lines = build_lines(blocklines, varmap);
				Line newl = Line(vars, lwc::is_equal);
				newl.linked_lines = built_block_lines;
				newl.loop = isloop;
				built_lines.push_back(newl);
			}
			else if (test(">")) {
				vector<variable> vars = lwc_get_vars(broken, varmap);
				vector<Line> built_block_lines = build_lines(blocklines, varmap);
				Line newl = Line(vars, lwc::is_greaterthan);
				newl.linked_lines = built_block_lines;
				newl.loop = isloop;
				built_lines.push_back(newl);
			}
			else if (test("<")) {
				vector<variable> vars = lwc_get_vars(broken, varmap);
				vector<Line> built_block_lines = build_lines(blocklines, varmap);
				Line newl = Line(vars, lwc::is_lessthan);
				newl.linked_lines = built_block_lines;
				newl.loop = isloop;
				built_lines.push_back(newl);
			}
			lnum += blocklines.size();
			continue;
		}
		
		if (test("+=")) {
			//vector<variable> vars = lwc_get_vars(broken, varmap);
			add_line(lwc::incrementby);
		}
		else if (test("-=")) {
			add_line(lwc::decrementby);
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
	fs.flush();
	clock_t start = clock();
	lwc::evaluate(build_lines(words));
	clock_t end = clock();
	double time = (double)(end - start);
	cout << "TIME: " << time << endl;
	cout << "Done Evaluating" << endl << endl; // let the user know we are done
	//cin >> ends;
	

	return 0;
}