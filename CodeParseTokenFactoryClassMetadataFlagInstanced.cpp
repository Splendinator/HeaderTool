#include "CodeParseTokenFactoryClassMetadataFlagInstanced.h"

#include "CodeParseTokenClassMetadataFlagBase.h"

bool CodeParseTokenFactoryClassMetadataFlagInstanced::IsKeyword(const std::string& keyword)
{
    return keyword == "Instanced";
}

CodeParseTokenBase* CodeParseTokenFactoryClassMetadataFlagInstanced::CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory)
{
    return new CodeParseTokenClassMetadataFlagInstanced();
}