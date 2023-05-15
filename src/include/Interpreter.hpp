#ifndef INCLUDE_INTERPRETER_HPP_
#define INCLUDE_INTERPRETER_HPP_

#include "Lexer.hpp"
#include <stack>

class Interpreter
{
	public:
		Interpreter(Lexer *lexer);
		std::vector<std::string> errors;
	private:
		Lexer *lexer;
		std::unordered_map<std::string, int> ints;
		std::unordered_map<std::string, std::string> strs;
		std::stack<std::string> mainStack;
		std::stack<int> callStack;
		void interpret();
		void call(int &i);
		int end();
		void ret(int &i);
		void print(int &i);
		void println(int &i);
		void intVars(int &i);
		void strVars(int &i);
		void set(int &i);
		void push(int &i);
		void del(int &i);
		void add(int &i);
		void sub(int &i);
		void mult(int &i);
		void div(int &i);
		void mod(int &i);
		void jmp(int &i);
		void jl(int &i);
		void je(int &i);
		void jg(int &i);
		void jne(int &i);
		void jle(int &i);
		void jge(int &i);
		void increment(int &i);
		void decrement(int &i);
		void input(int &i);
};

bool isInt(const std::string &str);

#endif /* INCLUDE_INTERPRETER_HPP_ */
