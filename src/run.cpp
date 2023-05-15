#include "include/run.hpp"

std::ostream& operator<<(std::ostream &os, const Interpreter& dt)
{
    for(std::string error : dt.errors)
    {
    	os << error << '\n';
    }
    return os;
}

int run(char* filename)
{
	Lexer lexer(filename);
	Interpreter interpreter(&lexer);
	std::cout << interpreter;
	lexer.free();
	return 0;
}
