#include "CodeParseTokenFactoryClassMetadataFlagAbstract.h"

#include "CodeParseTokenClassMetadataFlagBase.h"

bool CodeParseTokenFactoryClassMetadataFlagAbstract::IsKeyword(const std::string& keyword)
{
	return keyword == "Abstract";
}

CodeParseTokenBase* CodeParseTokenFactoryClassMetadataFlagAbstract::CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory)
{
	return new CodeParseTokenClassMetadataFlagAbstract();
}
