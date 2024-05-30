#pragma once
#include <iostream>

class RegularExpression
{
public:
	RegularExpression();

	std::string GetExpression() const;
	bool IsValid() const;

private:
	std::string expression;
};