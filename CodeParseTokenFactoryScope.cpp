#include "CodeParseTokenFactoryScope.h"

#include "CodeParseTokenScope.h"

bool CodeParseTokenFactoryScope::IsKeyword(const std::string& keyword)
{
	return keyword == "{";
}

std::string CodeParseTokenFactoryScope::GetEndString() const
{
	return "}";
}
