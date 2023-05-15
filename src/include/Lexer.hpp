#ifndef INCLUDE_LEXER_HPP_
#define INCLUDE_LEXER_HPP_

#include "Line.hpp"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

class Lexer
{
	public:
		Lexer(std::string filePath);
		void free();
		std::vector<Line*> lines;
		std::unordered_map<std::string, int> funcs, lbls;
	private:
		void funcs_and_lables();
		void lex(std::string filePath);
};

#endif /* INCLUDE_LEXER_HPP_ */
