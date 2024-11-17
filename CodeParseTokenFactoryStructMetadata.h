#pragma once

#include "CodeParseTokenFactoryBase.h"

class CodeParseTokenFactoryStructMetadata : public CodeParseTokenFactoryBase
{
public:
	//~ Begin CodeParseTokenFactoryBase Interface
	bool IsKeyword(const std::string& keyword) override;
	CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory) override;
	std::string GetEndString() const override;
	//~ End CodeParseTokenFactoryBase Interface
};
