#include "include/Lexer.hpp"

Lexer::Lexer(std::string filepath)
{
	lex(filepath);
	funcs_and_lables();
}
void Lexer::free()
{
	for (int i = 0; i < lines.size(); i++)
		delete lines[i];
}

void Lexer::lex(std::string filepath)
{
	int lineNumber = 0;
	std::ifstream myfile;
	myfile.open(filepath);
	std::string myline;
	if(myfile.is_open())
	{
		while (std::getline(myfile, myline))
		{
			int wordNumber = 0;
			std::string arg1, arg2, arg3, arg4, word;
			std::istringstream iss(myline);
			bool string = false;
			while (iss >> word)
			{
				if(wordNumber == 0)
				{
					arg1 = word;
					wordNumber++;
				}
				else if(wordNumber == 1)
				{
					if(word.front() == '\"')
					{
						arg2 = word;
						if(word.back() != '\"')
							string = true;
						else
							wordNumber++;
					}
					else
					{
						if(string)
						{
							arg2 += " " + word;
							if(word.back() == '\"')
							{
								string = false;
								wordNumber++;
							}

						}
						else
						{
							arg2 = word;
							wordNumber++;
						}
					}
				}
				else if(wordNumber == 2)
				{
					if(word.front() == '\"')
					{
						arg3 = word;
						if(word.back() != '\"')
							string = true;
						else
							wordNumber++;
					}
					else
					{
						if(string)
						{
							arg3 += " " + word;
							if(word.back() == '\"')
							{
								string = false;
								wordNumber++;
							}

						}
						else
						{
							string = false;
							arg3 = word;
							wordNumber++;
						}
					}
				}
				else if(wordNumber == 3)
				{
					if(word.front() == '\"')
					{
						arg4 = word;
						if(word.back() != '\"')
							string = true;
						else
							wordNumber++;
					}
					else
					{
						if(string)
						{
							arg4 += " " + word;
							if(word.back() == '\"')
							{
								string = false;
								wordNumber++;
							}

						}
						else
						{
							string = false;
							arg4 = word;
							wordNumber++;
						}
					}
				}
			}
			lines.push_back(new Line(arg1, arg2, arg3, arg4, lineNumber));
			lineNumber++;
		}
		myfile.close();

	}
	else
	{
		std::cerr << "Couldn't open file\n";
	}
}

void Lexer::funcs_and_lables()
{
	for (auto &line : lines)
	{
		auto a1 = line->mArg1, a2 = line->mArg2;
		int i = line->mLineNumber;
		if(a1 == "function")
		{
			funcs[a2] = i;
		}
		else if(a1 == "lbl")
		{
			lbls[a2] = i;
		}
	}
}
