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
//Testing git push from different machine

bool is_num(const string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}


void remove_whitespace(std::string &str) {
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

int main()
{
	std::clock_t start = clock();
	std::string file_name = "first_test.txt";
	std::fstream fs;
	std::string s;
	std::vector<std::string> words;
	
	std::cout << "Reading " << file_name << "...." << std::endl;

	fs.open(file_name.c_str());
	std::string line;
	while (std::getline(fs, line)) {
		remove_whitespace(line);
		if(line.size() > 0 && line[0] != '#')
			words.push_back(line);
	}
	fs.flush();
	std::unordered_map<std::string, lwc::variable> my_varmap;
	std::vector<lwc::LAST> root_scope(lwc::parse_from_slines(words));
	std::cout << "compilation complete" << std::endl;

	std::clock_t start_eval = clock();
	lwc::evaluate_lines(root_scope);
	clock_t end = clock();

	double fulltime = (double)(end - start);
	double evaltime = (double)(end - start_eval);
	std::cout << "Done Evaluating" << std::endl << std::endl;
	std::cout << "FULL RUNNING TIME: " << fulltime << std::endl;
	std::cout << "BUILDING TIME: " << fulltime - evaltime << std::endl;
	std::cout << "BUILT EVALUATION TIME: " << evaltime << std::endl;
	
	std::cout << std::endl;
	//getchar();
	return 0;
}