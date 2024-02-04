#include "CodeParseTokenFactoryScope.h"

#include "CodeParseTokenScope.h"

bool CodeParseTokenFactoryScope::IsKeyword(const std::string& keyword)
{
	return keyword == "{";
}

CodeParseTokenBase* CodeParseTokenFactoryScope::CreateToken(const std::string& keyword, std::stringstream& stream)
{
	return new CodeParseTokenScope();
}

std::string CodeParseTokenFactoryScope::GetEndString() const
{
	return "}";
}
