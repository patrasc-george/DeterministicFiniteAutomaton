#include <fstream>
#include <regex>
#include <boost/regex.hpp>

#include "RegularExpression.h"

RegularExpression::RegularExpression()
{
	std::ifstream f("input.txt");
	f >> expression;
	f.close();
}

std::string RegularExpression::GetExpression() const
{
	return expression;
}

bool RegularExpression::IsValid() const
{
	std::wstring aux((const wchar_t*)&expression[0], sizeof(char) / sizeof(wchar_t) * expression.size());
	try
	{
		std::wregex re(aux);
	}
	catch (const std::regex_error&)
	{
		return false;
	}
	return true;
}