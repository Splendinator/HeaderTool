#include "CodeParseTokenFactoryClass.h"

#include "CodeParseTokenClass.h"

bool CodeParseTokenFactoryClass::IsKeyword(const std::string& keyword)
{
	return keyword == "class";
}

CodeParseTokenBase* CodeParseTokenFactoryClass::CreateUDTToken(const std::string& udtName, const std::vector<std::string>& baseUdts)
{
	return new CodeParseTokenClass(udtName, baseUdts);
}
