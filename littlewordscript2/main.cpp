#include "lwc_builtins.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <string>
#include <chrono>
#include "lwc_Parser.h"
//Testing git push from different machine

void remove_whitespace(std::string &str) {
	string fin;
	for (const char& c : str) if (!isspace(c)) fin.push_back(c);
	str = fin;
}

int main(int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();
	std::string file_name = (argc > 1 ? argv[1] : "first_test.lwc");
	std::fstream fs;
	std::string s;
	std::vector<std::string> words;
	
	std::cout << "Reading " << file_name << "...." << std::endl;

	fs.open(file_name.c_str());
	if (fs.good()) {
		std::string line;
		while (std::getline(fs, line)) {
			remove_whitespace(line); //temporary solution. Lexer will have to handle this in future.
			if (line.size() > 0 && line[0] != '#')
				words.push_back(line);
		}
		fs.flush();
		std::unordered_map<std::string, lwc::variable> my_varmap;
		lwc::block_func root_scope(lwc::parse_from_slines(words));
		std::cout << "build complete" << std::endl;

		auto start_eval = std::chrono::high_resolution_clock::now();
		lwc::evaluate_lines(root_scope);
		auto end = std::chrono::high_resolution_clock::now();
		auto evaltime = (end - start_eval);
		auto fulltime = (end - start);
		std::cout << "Done Evaluating" << std::endl << std::endl;
		std::cout << "FULL RUNNING TIME: " << std::chrono::duration<double, std::milli>(fulltime).count() << std::endl;
		std::cout << "BUILDING TIME: " << std::chrono::duration<double, std::milli>(fulltime - evaltime).count() << std::endl;
		std::cout << "BUILT EVALUATION TIME: " << std::chrono::duration<double, std::milli>(evaltime).count() << std::endl;

		std::cout << std::endl;
	}
	else {
		std::cout << "File: " << file_name << " could not be opened." << std::endl;
	}
	getchar(); //pause execution so breakpoint is unneeded. Should be disabled in performance checks of course.
	return 0;
}