#pragma once

#include "CodeParseTokenFactoryBase.h"

/**
 * Factory to parse code for enum values and create the CodeParseTokenEnumValue 
 */
class CodeParseTokenFactoryEnumValue : public CodeParseTokenFactoryBase
{
public:
	//~ Begin CodeParseTokenFactoryBase Interface
	bool IsKeyword(const std::string& keyword) override;
	CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory) override;
	//~ End CodeParseTokenFactoryBase Interface
};
