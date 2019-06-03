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
#include "lwc_Parser.h"
#include "lwc_Evaluator.h"
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
	//std::vector<lwc::Line> final_lines = build_lines(words, my_varmap);
	//lwc::Evaluator my_eval = lwc::Evaluator();
	std::cout << "compilation complete" << std::endl;
	std::clock_t start_eval = clock();
	//my_eval.evaluate(final_lines);
	clock_t end = clock();
	double fulltime = (double)(end - start);
	double evaltime = (double)(end - start_eval);
	std::cout << "Done Evaluating" << std::endl << std::endl;
	std::cout << "FULL RUNNING TIME: " << fulltime << std::endl;
	std::cout << "BUILDING TIME: " << fulltime - evaltime << std::endl;
	std::cout << "BUILT EVALUATION TIME: " << evaltime << std::endl;
	lwc::TokenQueue tq("(1*2)+(6/2)+9");

	std::queue<lwc::ParseToken> qq(lwc::shunting_yard(tq));
	lwc::LAST mylast(qq, lwc::global);
	
	while (!qq.empty()) {
		std::string spush = qq.front().val;
		if (qq.front().tt == lwc::TokenType::op)
			spush = "OP";
		std::cout << spush  << " ";
		qq.pop();
	}
	
	std::cout << std::endl;
	//getchar();
	return 0;
}