#include "include/Interpreter.hpp"
#include <cstdlib>
#include <ctime>

Interpreter::Interpreter(Lexer *lexer) : lexer(lexer)
{
	interpret();
}

void Interpreter::interpret()
{
	int i = -1;
	bool function = false;
	if(lexer->funcs.count("main") != 0) i = lexer->funcs.at("main");
	for (; ++i < lexer->lines.size();)
	{
		std::string a1 = lexer->lines[i]->mArg1, a2 = lexer->lines[i]->mArg2,
				a3 = lexer->lines[i]->mArg3, a4 = lexer->lines[i]->mArg4;
		if(function)
		{
			if(a1 == "end") function = false;
		}
		else if(a1 == "call")
		{
			call(i);
		}
		else if(a1 == "end")
		{
			i = end();
			if(i == -1) break;
		}
		else if(a1 == "print")
		{
			print(i);
		}
		else if(a1 == "println")
		{
			println(i);
		}
		else if(a1 == "exit")
			break;
		else if(a2 == ":int=")
		{
			intVars(i);
		}
		else if(a2 == ":str=")
		{
			strVars(i);
		}
		else if(a2 == ":=")
		{
			set(i);
		}
		else if(a1 == "ret")
		{
			ret(i);
			if(i == -1) break;
		}
		else if(a1 == "delete")
		{
			del(i);
		}
		else if(a1 == "push")
		{
			push(i);
		}
		else if(a2 == "+=")
		{
			add(i);
		}
		else if(a2 == "%=")
		{
			mod(i);
		}
		else if(a2 == "-=")
		{
			sub(i);
		}
		else if(a2 == "*=")
		{
			mult(i);
		}
		else if(a2 == "/=")
		{
			div(i);
		}
		else if(a1 == "jmp")
		{
			jmp(i);
		}
		else if(a1 == "jl")
		{
			jl(i);
		}
		else if(a1 == "jg")
		{
			jg(i);
		}
		else if(a1 == "je")
		{
			je(i);
		}
		else if(a1 == "jne")
		{
			jne(i);
		}
		else if(a1 == "jle")
		{
			jle(i);
		}
		else if(a1 == "jge")
		{
			jge(i);
		}
		else if(a2 == "++")
		{
			increment(i);
		}
		else if(a2 == "--")
		{
			decrement(i);
		}
		else if(a1 == "input")
		{
			input(i);
		}
	}
	std::cout << std::endl;
}

void Interpreter::call(int &i)
{
	if(lexer->funcs.count(lexer->lines[i]->mArg2) != 0)
	{
		callStack.push(*(&i));
		i = lexer->funcs.at(lexer->lines[i]->mArg2);
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":" + "\n\t function \'"
						+ lexer->lines[i]->mArg2 + "\' does not exist.");
	}
}

int Interpreter::end()
{
	if(callStack.empty()) return -1;
	int i = callStack.top();
	callStack.pop();
	return i;
}

void Interpreter::ret(int &i)
{
	if(callStack.empty())
		i = -1;
	else
	{
		if(lexer->lines[i]->mArg2 != "")
		{
			if(isInt(lexer->lines[i]->mArg2))
			{
				mainStack.push(lexer->lines[i]->mArg2);
			}
			else if(ints.count(lexer->lines[i]->mArg2))
			{
				mainStack.push(std::to_string(ints.at(lexer->lines[i]->mArg2)));
			}
			else if(strs.count(lexer->lines[i]->mArg2))
			{
				mainStack.push(strs.at(lexer->lines[i]->mArg2));
			}
			else if(lexer->lines[i]->mArg2.front() == '\"'
					|| lexer->lines[i]->mArg2.back() == '\"')
			{
				if(lexer->lines[i]->mArg2.front() != '\"'
						|| lexer->lines[i]->mArg2.back() != '\"')
					errors.push_back(
							"Error at line " + std::to_string(i) + ":"
									+ "\n\t Strings must start and end with \"");
				else
				{
					mainStack.push(
							lexer->lines[i]->mArg2.substr(1,
									lexer->lines[i]->mArg2.length() - 2));
				}
			}
			else
			{
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Unknown word \'"
								+ lexer->lines[i]->mArg2 + "\'.");
			}
		}
		i = callStack.top();
		callStack.pop();
	}

}

void Interpreter::print(int &i)
{
	if(isInt(lexer->lines[i]->mArg2))
	{
		std::cout << lexer->lines[i]->mArg2;
	}
	else if(ints.count(lexer->lines[i]->mArg2))
	{
		std::cout << std::to_string(ints.at(lexer->lines[i]->mArg2));
	}
	else if(strs.count(lexer->lines[i]->mArg2))
	{
		std::cout << strs.at(lexer->lines[i]->mArg2);
	}
	else if(lexer->lines[i]->mArg2.front() == '\"'
			|| lexer->lines[i]->mArg2.back() == '\"')
	{
		if(lexer->lines[i]->mArg2.front() != '\"'
				|| lexer->lines[i]->mArg2.back() != '\"')
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Strings must start and end with \"");
		else
		{
			std::cout
					<< lexer->lines[i]->mArg2.substr(1,
							lexer->lines[i]->mArg2.length() - 2);
		}
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg2
						+ "\'.");
	}
}

void Interpreter::println(int &i)
{
	if(isInt(lexer->lines[i]->mArg2))
	{
		std::cout << lexer->lines[i]->mArg2 << "\n";
	}
	else if(ints.count(lexer->lines[i]->mArg2))
	{
		std::cout << std::to_string(ints.at(lexer->lines[i]->mArg2)) << "\n";
	}
	else if(strs.count(lexer->lines[i]->mArg2))
	{
		std::cout << strs.at(lexer->lines[i]->mArg2) << "\n";
	}
	else if(lexer->lines[i]->mArg2.front() == '\"'
			|| lexer->lines[i]->mArg2.back() == '\"')
	{
		if(lexer->lines[i]->mArg2.front() != '\"'
				|| lexer->lines[i]->mArg2.back() != '\"')
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Strings must start and end with \"");
		else
		{
			std::cout
					<< lexer->lines[i]->mArg2.substr(1,
							lexer->lines[i]->mArg2.length() - 2) << "\n";
		}
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg2
						+ "\'.");
	}
}

void Interpreter::strVars(int &i)
{
	if(strs.count(lexer->lines[i]->mArg1) != 0)
	{
		errors.push_back(
				"Warning at line " + std::to_string(i) + ":" + "\n\t \'"
						+ lexer->lines[i]->mArg1 + "\' already exists.");
	}
	if(isInt(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of string to a int literal");
	}
	else if(ints.count(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of string to a string");
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of string to a string literal");

	}
	else
	{
		if(isInt(lexer->lines[i]->mArg3))
		{
			strs[lexer->lines[i]->mArg3] = lexer->lines[i]->mArg3;
		}
		else if(ints.count(lexer->lines[i]->mArg3))
		{
			strs[lexer->lines[i]->mArg3] = ints.at(lexer->lines[i]->mArg3);
		}
		else if(strs.count(lexer->lines[i]->mArg3))
		{
			strs[lexer->lines[i]->mArg3] = strs.at(lexer->lines[i]->mArg3);
		}
		else if(lexer->lines[i]->mArg3.front() == '\"'
				|| lexer->lines[i]->mArg3.back() == '\"')
		{
			if(lexer->lines[i]->mArg3.front() != '\"'
					|| lexer->lines[i]->mArg3.back() != '\"')
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Strings must start and end with \"");
			else
			{
				strs[lexer->lines[i]->mArg1] = lexer->lines[i]->mArg3.substr(1,
						lexer->lines[i]->mArg3.length() - 2);
			}
		}
		else if(lexer->lines[i]->mArg3 == "pop")
		{
			if(mainStack.empty())
			{
				strs[lexer->lines[i]->mArg1] = "";
			}
			else
			{
				strs[lexer->lines[i]->mArg1] = mainStack.top();
				mainStack.pop();
			}
		}
		else if(lexer->lines[i]->mArg3 == "rand")
		{
			std::srand(std::time(nullptr));
			strs[lexer->lines[i]->mArg1] = std::to_string(std::rand());
		}
		else
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Unknown word \'" + lexer->lines[i]->mArg3
							+ "\'.");
		}
	}
}

void Interpreter::intVars(int &i)
{
	if(ints.count(lexer->lines[i]->mArg1) != 0)
	{
		errors.push_back(
				"Warning at line " + std::to_string(i) + ":" + "\n\t \'"
						+ lexer->lines[i]->mArg1 + "\' already exists.");
	}
	if(isInt(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a int literal");
	}
	else if(strs.count(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string");
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string literal");

	}
	else
	{
		if(isInt(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] = std::stoi(lexer->lines[i]->mArg3);
		}
		else if(ints.count(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] = ints.at(lexer->lines[i]->mArg3);
		}
		else if(strs.count(lexer->lines[i]->mArg3))
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not set an int to a string");
		}
		else if(lexer->lines[i]->mArg3.front() == '\"'
				|| lexer->lines[i]->mArg3.back() == '\"')
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not set an int to a string literal");
		}
		else if(lexer->lines[i]->mArg3 == "pop")
		{
			if(mainStack.empty())
			{
				ints[lexer->lines[i]->mArg1] = 0;
			}
			else if(isInt(mainStack.top()))
			{
				ints[lexer->lines[i]->mArg1] = std::stoi(mainStack.top());
				mainStack.pop();
			}
			else
			{
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Can not set an int to a string");
			}
		}
		else if(lexer->lines[i]->mArg3 == "rand")
		{
			std::srand(std::time(nullptr));
			ints[lexer->lines[i]->mArg1] = std::rand();
		}
		else
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Unknown word \'" + lexer->lines[i]->mArg3
							+ "\'.");
		}
	}

}

void Interpreter::set(int &i)
{
	if(ints.count(lexer->lines[i]->mArg1) != 0)
	{
		if(isInt(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] = std::stoi(lexer->lines[i]->mArg3);
		}
		else if(ints.count(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] = ints.at(lexer->lines[i]->mArg3);
		}
		else if(strs.count(lexer->lines[i]->mArg3))
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not set an int to a string");
		}
		else if(lexer->lines[i]->mArg3.front() == '\"'
				|| lexer->lines[i]->mArg3.back() == '\"')
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not set an int to a string literal");
		}
		else if(lexer->lines[i]->mArg3 == "pop")
		{
			if(mainStack.empty())
			{
				ints[lexer->lines[i]->mArg1] = 0;
			}
			else if(isInt(mainStack.top()))
			{
				ints[lexer->lines[i]->mArg1] = std::stoi(mainStack.top());
				mainStack.pop();
			}
			else
			{
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Can not set an int to a string");
			}
		}
		else if(lexer->lines[i]->mArg3 == "rand")
		{
			std::srand(std::time(nullptr));
			ints[lexer->lines[i]->mArg1] = std::rand();
		}
		else
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Unknown word \'" + lexer->lines[i]->mArg3
							+ "\'.");
		}
	}
	else if(strs.count(lexer->lines[i]->mArg1) != 0)
	{
		if(isInt(lexer->lines[i]->mArg3))
		{
			strs[lexer->lines[i]->mArg3] = lexer->lines[i]->mArg3;
		}
		else if(ints.count(lexer->lines[i]->mArg3))
		{
			strs[lexer->lines[i]->mArg3] = ints.at(lexer->lines[i]->mArg3);
		}
		else if(strs.count(lexer->lines[i]->mArg3))
		{
			strs[lexer->lines[i]->mArg3] = strs.at(lexer->lines[i]->mArg3);
		}
		else if(lexer->lines[i]->mArg3.front() == '\"'
				|| lexer->lines[i]->mArg3.back() == '\"')
		{
			if(lexer->lines[i]->mArg3.front() != '\"'
					|| lexer->lines[i]->mArg3.back() != '\"')
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Strings must start and end with \"");
			else
			{
				strs[lexer->lines[i]->mArg1] = lexer->lines[i]->mArg3.substr(1,
						lexer->lines[i]->mArg3.length() - 2);
			}
		}
		else if(lexer->lines[i]->mArg3 == "pop")
		{
			if(mainStack.empty())
			{
				strs[lexer->lines[i]->mArg1] = "";
			}
			else
			{
				strs[lexer->lines[i]->mArg1] = mainStack.top();
				mainStack.pop();
			}
		}
		else if(lexer->lines[i]->mArg3 == "rand")
		{
			std::srand(std::time(nullptr));
			strs[lexer->lines[i]->mArg1] = std::to_string(std::rand());
		}
		else
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Unknown word \'" + lexer->lines[i]->mArg3
							+ "\'.");
		}
	}
	else if(isInt(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a int literal");
	}
	else if(strs.count(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string");
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string literal");

	}
}

void Interpreter::add(int &i)
{
	if(ints.count(lexer->lines[i]->mArg1) != 0)
	{
		if(isInt(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] += std::stoi(lexer->lines[i]->mArg3);
		}
		else if(ints.count(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] += ints.at(lexer->lines[i]->mArg3);
		}
		else if(strs.count(lexer->lines[i]->mArg3))
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not add an int to a string");
		}
		else if(lexer->lines[i]->mArg3.front() == '\"'
				|| lexer->lines[i]->mArg3.back() == '\"')
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not set add int to a string literal");
		}
		else if(lexer->lines[i]->mArg3 == "pop")
		{
			if(mainStack.empty())
			{
				ints[lexer->lines[i]->mArg1] += 0;
			}
			else if(isInt(mainStack.top()))
			{
				ints[lexer->lines[i]->mArg1] += std::stoi(mainStack.top());
				mainStack.pop();
			}
			else
			{
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Can not set an int to a string");
			}
		}
		else if(lexer->lines[i]->mArg3 == "rand")
		{
			std::srand(std::time(nullptr));
			ints[lexer->lines[i]->mArg1] += std::rand();
		}
		else
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Unknown word \'" + lexer->lines[i]->mArg3
							+ "\'.");
		}
	}
	else if(strs.count(lexer->lines[i]->mArg1) != 0)
	{
		if(isInt(lexer->lines[i]->mArg3))
		{
			strs[lexer->lines[i]->mArg3] += lexer->lines[i]->mArg3;
		}
		else if(ints.count(lexer->lines[i]->mArg3))
		{
			strs[lexer->lines[i]->mArg3] += ints.at(lexer->lines[i]->mArg3);
		}
		else if(strs.count(lexer->lines[i]->mArg3))
		{
			strs[lexer->lines[i]->mArg3] += strs.at(lexer->lines[i]->mArg3);
		}
		else if(lexer->lines[i]->mArg3.front() == '\"'
				|| lexer->lines[i]->mArg3.back() == '\"')
		{
			if(lexer->lines[i]->mArg3.front() != '\"'
					|| lexer->lines[i]->mArg3.back() != '\"')
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Strings must start and end with \"");
			else
			{
				strs[lexer->lines[i]->mArg1] += lexer->lines[i]->mArg3.substr(1,
						lexer->lines[i]->mArg3.length() - 2);
			}
		}
		else if(lexer->lines[i]->mArg3 == "pop")
		{
			if(mainStack.empty())
			{
				strs[lexer->lines[i]->mArg1] += "";
			}
			else
			{
				strs[lexer->lines[i]->mArg1] = mainStack.top();
				mainStack.pop();
			}
		}
		else if(lexer->lines[i]->mArg3 == "rand")
		{
			std::srand(std::time(nullptr));
			strs[lexer->lines[i]->mArg1] += std::to_string(std::rand());
		}
		else
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Unknown word \'" + lexer->lines[i]->mArg3
							+ "\'.");
		}
	}
	else if(isInt(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a int literal");
	}
	else if(strs.count(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string");
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string literal");

	}
}

void Interpreter::sub(int &i)
{
	if(ints.count(lexer->lines[i]->mArg1) != 0)
	{
		if(isInt(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] -= std::stoi(lexer->lines[i]->mArg3);
		}
		else if(ints.count(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] -= ints.at(lexer->lines[i]->mArg3);
		}
		else if(strs.count(lexer->lines[i]->mArg3))
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not subtract an int to a string");
		}
		else if(lexer->lines[i]->mArg3.front() == '\"'
				|| lexer->lines[i]->mArg3.back() == '\"')
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not set subtract int to a string literal");
		}
		else if(lexer->lines[i]->mArg3 == "pop")
		{
			if(mainStack.empty())
			{
				ints[lexer->lines[i]->mArg1] -= 0;
			}
			else if(isInt(mainStack.top()))
			{
				ints[lexer->lines[i]->mArg1] -= std::stoi(mainStack.top());
				mainStack.pop();
			}
			else
			{
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Can not subtract an int to a string");
			}
		}
		else if(lexer->lines[i]->mArg3 == "rand")
		{
			std::srand(std::time(nullptr));
			ints[lexer->lines[i]->mArg1] -= std::rand();
		}
		else
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Unknown word \'" + lexer->lines[i]->mArg3
							+ "\'.");
		}
	}
	else if(strs.count(lexer->lines[i]->mArg1) != 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not subtract a string with any value");
	}
	else if(isInt(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a int literal");
	}
	else if(strs.count(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string");
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string literal");

	}
}

void Interpreter::mult(int &i)
{
	if(ints.count(lexer->lines[i]->mArg1) != 0)
	{
		if(isInt(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] *= std::stoi(lexer->lines[i]->mArg3);
		}
		else if(ints.count(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] *= ints.at(lexer->lines[i]->mArg3);
		}
		else if(strs.count(lexer->lines[i]->mArg3))
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not multiply an int to a string");
		}
		else if(lexer->lines[i]->mArg3.front() == '\"'
				|| lexer->lines[i]->mArg3.back() == '\"')
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not set multiply int to a string literal");
		}
		else if(lexer->lines[i]->mArg3 == "pop")
		{
			if(mainStack.empty())
			{
				ints[lexer->lines[i]->mArg1] *= 0;
			}
			else if(isInt(mainStack.top()))
			{
				ints[lexer->lines[i]->mArg1] *= std::stoi(mainStack.top());
				mainStack.pop();
			}
			else
			{
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Can not multiply an int to a string");
			}
		}
		else if(lexer->lines[i]->mArg3 == "rand")
		{
			std::srand(std::time(nullptr));
			ints[lexer->lines[i]->mArg1] *= std::rand();
		}
		else
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Unknown word \'" + lexer->lines[i]->mArg3
							+ "\'.");
		}
	}
	else if(strs.count(lexer->lines[i]->mArg1) != 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not multiply a string with any value");
	}
	else if(isInt(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a int literal");
	}
	else if(strs.count(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string");
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string literal");

	}
}

void Interpreter::div(int &i)
{
	if(ints.count(lexer->lines[i]->mArg1) != 0)
	{
		if(isInt(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] /= std::stoi(lexer->lines[i]->mArg3);
		}
		else if(ints.count(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] /= ints.at(lexer->lines[i]->mArg3);
		}
		else if(strs.count(lexer->lines[i]->mArg3))
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not divide an int to a string");
		}
		else if(lexer->lines[i]->mArg3.front() == '\"'
				|| lexer->lines[i]->mArg3.back() == '\"')
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not set divide int to a string literal");
		}
		else if(lexer->lines[i]->mArg3 == "pop")
		{
			if(mainStack.empty())
			{
				ints[lexer->lines[i]->mArg1] -= 0;
			}
			else if(isInt(mainStack.top()))
			{
				ints[lexer->lines[i]->mArg1] /= std::stoi(mainStack.top());
				mainStack.pop();
			}
			else
			{
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Can not divide an int to a string");
			}
		}
		else if(lexer->lines[i]->mArg3 == "rand")
		{
			std::srand(std::time(nullptr));
			ints[lexer->lines[i]->mArg1] /= std::rand();
		}
		else
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Unknown word \'" + lexer->lines[i]->mArg3
							+ "\'.");
		}
	}
	else if(strs.count(lexer->lines[i]->mArg1) != 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not divide a string with any value");
	}
	else if(isInt(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a int literal");
	}
	else if(strs.count(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string");
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string literal");

	}
}

void Interpreter::mod(int &i)
{
	if(ints.count(lexer->lines[i]->mArg1) != 0)
	{
		if(isInt(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] %= std::stoi(lexer->lines[i]->mArg3);
		}
		else if(ints.count(lexer->lines[i]->mArg3))
		{
			ints[lexer->lines[i]->mArg1] %= ints.at(lexer->lines[i]->mArg3);
		}
		else if(strs.count(lexer->lines[i]->mArg3))
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not mod an int to a string");
		}
		else if(lexer->lines[i]->mArg3.front() == '\"'
				|| lexer->lines[i]->mArg3.back() == '\"')
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Can not set mod int to a string literal");
		}
		else if(lexer->lines[i]->mArg3 == "pop")
		{
			if(mainStack.empty())
			{
				ints[lexer->lines[i]->mArg1] += 0;
			}
			else if(isInt(mainStack.top()))
			{
				ints[lexer->lines[i]->mArg1] %= std::stoi(mainStack.top());
				mainStack.pop();
			}
			else
			{
				errors.push_back(
						"Error at line " + std::to_string(i) + ":"
								+ "\n\t Can not mod an int to a string");
			}
		}
		else if(lexer->lines[i]->mArg3 == "rand")
		{
			std::srand(std::time(nullptr));
			ints[lexer->lines[i]->mArg1] %= std::rand();
		}
		else
		{
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Unknown word \'" + lexer->lines[i]->mArg3
							+ "\'.");
		}
	}
	else if(strs.count(lexer->lines[i]->mArg1) != 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not mod a string with any value");
	}
	else if(isInt(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a int literal");
	}
	else if(strs.count(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string");
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not set name of int to a string literal");

	}
}

void Interpreter::push(int &i)
{
	if(strs.count(lexer->lines[i]->mArg2) != 0)
	{
		mainStack.push(strs.at(lexer->lines[i]->mArg2));
	}
	else if(isInt(lexer->lines[i]->mArg2))
	{
		mainStack.push(lexer->lines[i]->mArg2);
	}
	else if(ints.count(lexer->lines[i]->mArg2))
	{
		mainStack.push(std::to_string(ints.at(lexer->lines[i]->mArg2)));
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		if(lexer->lines[i]->mArg2.front() != '\"'
				|| lexer->lines[i]->mArg2.back() != '\"')
			errors.push_back(
					"Error at line " + std::to_string(i) + ":"
							+ "\n\t Strings must start and end with \"");
		else
		{
			mainStack.push(
					lexer->lines[i]->mArg2.substr(1,
							lexer->lines[i]->mArg2.length() - 2));
		}
	}
	else if(lexer->lines[i]->mArg2 == "rand")
	{
		std::srand(std::time(nullptr));
		mainStack.push(std::to_string(std::rand()));
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg2
						+ "\'.");
	}
}

void Interpreter::jmp(int &i)
{
	if(lexer->lines[i]->mArg2 == "")
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Need a label to jump to.");
	}
	else if(lexer->lbls.count(lexer->lines[i]->mArg2) == 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":" + "\n\t Label \'"
						+ lexer->lines[i]->mArg2 + "\' does not exist.");
	}
	else
	{
		i = lexer->lbls.at(lexer->lines[i]->mArg2);
	}
}

void Interpreter::jl(int &i)
{
	if(lexer->lines[i]->mArg4 == "")
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Need a label to jump to.");
	}
	else if(lexer->lbls.count(lexer->lines[i]->mArg4) == 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":" + "\n\t Label \'"
						+ lexer->lines[i]->mArg4 + "\' does not exist.");
	}
	else if(isInt(lexer->lines[i]->mArg2) && isInt(lexer->lines[i]->mArg3))
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				< std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(isInt(lexer->lines[i]->mArg2)
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				< ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& isInt(lexer->lines[i]->mArg3))
	{
		i = ints.at(lexer->lines[i]->mArg2)
				< std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = ints.at(lexer->lines[i]->mArg2) < ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(lexer->lines[i]->mArg2.front() == '\"'
			|| lexer->lines[i]->mArg2.back() == '\"'
			|| lexer->lines[i]->mArg3.front() == '\"'
			|| lexer->lines[i]->mArg3.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string literal.");
	}
	else if(strs.count(lexer->lines[i]->mArg2) != 0
			|| strs.count(lexer->lines[i]->mArg3) != 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string.");
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg2
						+ "\'.");
	}
}

void Interpreter::jg(int &i)
{
	if(lexer->lines[i]->mArg4 == "")
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Need a label to jump to.");
	}
	else if(lexer->lbls.count(lexer->lines[i]->mArg4) == 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":" + "\n\t Label \'"
						+ lexer->lines[i]->mArg4 + "\' does not exist.");
	}
	else if(isInt(lexer->lines[i]->mArg2) && isInt(lexer->lines[i]->mArg3))
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				> std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(isInt(lexer->lines[i]->mArg2)
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				> ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& isInt(lexer->lines[i]->mArg3))
	{
		i = ints.at(lexer->lines[i]->mArg2)
				> std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = ints.at(lexer->lines[i]->mArg2) > ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(lexer->lines[i]->mArg2.front() == '\"'
			|| lexer->lines[i]->mArg2.back() == '\"'
			|| lexer->lines[i]->mArg3.front() == '\"'
			|| lexer->lines[i]->mArg3.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string literal.");
	}
	else if(strs.count(lexer->lines[i]->mArg2) != 0
			|| strs.count(lexer->lines[i]->mArg3) != 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string.");
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg2
						+ "\'.");
	}
}

void Interpreter::je(int &i)
{
	if(lexer->lines[i]->mArg4 == "")
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Need a label to jump to.");
	}
	else if(lexer->lbls.count(lexer->lines[i]->mArg4) == 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":" + "\n\t Label \'"
						+ lexer->lines[i]->mArg4 + "\' does not exist.");
	}
	else if(isInt(lexer->lines[i]->mArg2) && isInt(lexer->lines[i]->mArg3))
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				== std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(isInt(lexer->lines[i]->mArg2)
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				== ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& isInt(lexer->lines[i]->mArg3))
	{
		i = ints.at(lexer->lines[i]->mArg2)
				== std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = ints.at(lexer->lines[i]->mArg2) == ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(lexer->lines[i]->mArg2.front() == '\"'
			|| lexer->lines[i]->mArg2.back() == '\"'
			|| lexer->lines[i]->mArg3.front() == '\"'
			|| lexer->lines[i]->mArg3.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string literal.");
	}
	else if(strs.count(lexer->lines[i]->mArg2) != 0
			|| strs.count(lexer->lines[i]->mArg3) != 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string.");
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg2
						+ "\'.");
	}
}

void Interpreter::jne(int &i)
{
	if(lexer->lines[i]->mArg4 == "")
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Need a label to jump to.");
	}
	else if(lexer->lbls.count(lexer->lines[i]->mArg4) == 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":" + "\n\t Label \'"
						+ lexer->lines[i]->mArg4 + "\' does not exist.");
	}
	else if(isInt(lexer->lines[i]->mArg2) && isInt(lexer->lines[i]->mArg3))
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				!= std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(isInt(lexer->lines[i]->mArg2)
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				!= ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& isInt(lexer->lines[i]->mArg3))
	{
		i = ints.at(lexer->lines[i]->mArg2)
				!= std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = ints.at(lexer->lines[i]->mArg2) != ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(lexer->lines[i]->mArg2.front() == '\"'
			|| lexer->lines[i]->mArg2.back() == '\"'
			|| lexer->lines[i]->mArg3.front() == '\"'
			|| lexer->lines[i]->mArg3.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string literal.");
	}
	else if(strs.count(lexer->lines[i]->mArg2) != 0
			|| strs.count(lexer->lines[i]->mArg3) != 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string.");
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg2
						+ "\'.");
	}
}

void Interpreter::jle(int &i)
{
	if(lexer->lines[i]->mArg4 == "")
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Need a label to jump to.");
	}
	else if(lexer->lbls.count(lexer->lines[i]->mArg4) == 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":" + "\n\t Label \'"
						+ lexer->lines[i]->mArg4 + "\' does not exist.");
	}
	else if(isInt(lexer->lines[i]->mArg2) && isInt(lexer->lines[i]->mArg3))
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				<= std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(isInt(lexer->lines[i]->mArg2)
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				<= ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& isInt(lexer->lines[i]->mArg3))
	{
		i = ints.at(lexer->lines[i]->mArg2)
				<= std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = ints.at(lexer->lines[i]->mArg2) <= ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(lexer->lines[i]->mArg2.front() == '\"'
			|| lexer->lines[i]->mArg2.back() == '\"'
			|| lexer->lines[i]->mArg3.front() == '\"'
			|| lexer->lines[i]->mArg3.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string literal.");
	}
	else if(strs.count(lexer->lines[i]->mArg2) != 0
			|| strs.count(lexer->lines[i]->mArg3) != 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string.");
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg2
						+ "\'.");
	}
}

void Interpreter::jge(int &i)
{
	if(lexer->lines[i]->mArg4 == "")
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Need a label to jump to.");
	}
	else if(lexer->lbls.count(lexer->lines[i]->mArg4) == 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":" + "\n\t Label \'"
						+ lexer->lines[i]->mArg4 + "\' does not exist.");
	}
	else if(isInt(lexer->lines[i]->mArg2) && isInt(lexer->lines[i]->mArg3))
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				>= std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(isInt(lexer->lines[i]->mArg2)
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = std::stoi(lexer->lines[i]->mArg2)
				>= ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& isInt(lexer->lines[i]->mArg3))
	{
		i = ints.at(lexer->lines[i]->mArg2)
				>= std::stoi(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(ints.count(lexer->lines[i]->mArg2) != 0
			&& ints.count(lexer->lines[i]->mArg3) != 0)
	{
		i = ints.at(lexer->lines[i]->mArg2) >= ints.at(lexer->lines[i]->mArg3) ?
				lexer->lbls.at(lexer->lines[i]->mArg4) : i;
	}
	else if(lexer->lines[i]->mArg2.front() == '\"'
			|| lexer->lines[i]->mArg2.back() == '\"'
			|| lexer->lines[i]->mArg3.front() == '\"'
			|| lexer->lines[i]->mArg3.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string literal.");
	}
	else if(strs.count(lexer->lines[i]->mArg2) != 0
			|| strs.count(lexer->lines[i]->mArg3) != 0)
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not compare a string.");
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg2
						+ "\'.");
	}
}

void Interpreter::del(int &i)
{
	if(isInt(lexer->lines[i]->mArg2))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t can not delete a int literal.");
	}
	else if(ints.count(lexer->lines[i]->mArg2))
	{
		ints.erase(ints.find(lexer->lines[i]->mArg2));
	}
	else if(strs.count(lexer->lines[i]->mArg2))
	{
		strs.erase(strs.find(lexer->lines[i]->mArg2));
	}
	else if(lexer->lines[i]->mArg2.front() == '\"'
			|| lexer->lines[i]->mArg2.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\tC Can not delete a string literal.");
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg2
						+ "\'.");
	}
}

void Interpreter::increment(int &i)
{
	if(isInt(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not increment an int literal.");
	}
	else if(ints.count(lexer->lines[i]->mArg1))
	{
		ints[lexer->lines[i]->mArg1]++;
	}
	else if(strs.count(lexer->lines[i]->mArg1))
	{
		strs[lexer->lines[i]->mArg1] += 1;
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not increment a string literal.");
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg1
						+ "\'.");
	}
}

void Interpreter::decrement(int &i)
{
	if(isInt(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not decrement an int literal.");
	}
	else if(ints.count(lexer->lines[i]->mArg1))
	{
		ints[lexer->lines[i]->mArg1]--;
	}
	else if(strs.count(lexer->lines[i]->mArg1))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not decrement an string.");
	}
	else if(lexer->lines[i]->mArg1.front() == '\"'
			|| lexer->lines[i]->mArg1.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not decrement a string literal.");
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg1
						+ "\'.");
	}
}

void Interpreter::input(int &i)
{
	if(isInt(lexer->lines[i]->mArg2))
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not change the value of an int literal.");
	}
	else if(ints.count(lexer->lines[i]->mArg2))
	{
		std::cin >> ints[lexer->lines[i]->mArg2];
	}
	else if(strs.count(lexer->lines[i]->mArg2))
	{
		std::cin.ignore(); // Ignore any remaining newline character
		std::getline(std::cin, strs[lexer->lines[i]->mArg2]);
	}
	else if(lexer->lines[i]->mArg2.front() == '\"'
			|| lexer->lines[i]->mArg2.back() == '\"')
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Can not change the value of a string literal.");
	}
	else
	{
		errors.push_back(
				"Error at line " + std::to_string(i) + ":"
						+ "\n\t Unknown word \'" + lexer->lines[i]->mArg1
						+ "\'.");
	}
}

bool isInt(const std::string &str)
{
	if(str.empty()) return false;

	// Check for optional sign (+ or -)
	size_t start_pos = 0;
	if(str[0] == '-' || str[0] == '+') start_pos = 1;

	// Check if the remaining characters are digits
	for (size_t i = start_pos; i < str.length(); ++i)
	{
		if(!std::isdigit(str[i])) return false;
	}

	return true;
}
