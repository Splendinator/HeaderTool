#include "CodeParseTokenFactoryClassMetadataFlagEditInlineNew.h"

#include "CodeParseTokenClassMetadataFlagBase.h"

bool CodeParseTokenFactoryClassMetadataFlagEditInlineNew::IsKeyword(const std::string& keyword)
{
	return keyword == "EditInlineNew";
}

CodeParseTokenBase* CodeParseTokenFactoryClassMetadataFlagEditInlineNew::CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory)
{
	return new CodeParseTokenClassMetadataFlagEditInlineNew();
}
