#ifndef INCLUDE_LINE_HPP_
#define INCLUDE_LINE_HPP_

#include <string>

struct Line
{
		Line(std::string arg1, std::string arg2, std::string arg3,
				std::string arg4, int lineNumber);
		const std::string mArg1, mArg2, mArg3, mArg4;
		const int mLineNumber;
};

#endif /* INCLUDE_LINE_HPP_ */
