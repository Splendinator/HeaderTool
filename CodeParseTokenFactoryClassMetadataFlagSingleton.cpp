#include "CodeParseTokenFactoryClassMetadataFlagSingleton.h"

#include "CodeParseTokenClassMetadataFlagBase.h"

bool CodeParseTokenFactoryClassMetadataFlagSingleton::IsKeyword(const std::string& keyword)
{
    return keyword == "Singleton";
}

CodeParseTokenBase* CodeParseTokenFactoryClassMetadataFlagSingleton::CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory)
{
    return new CodeParseTokenClassMetadataFlagSingleton();
}