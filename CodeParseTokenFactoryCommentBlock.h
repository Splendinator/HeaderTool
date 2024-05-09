#pragma once

#include "CodeParseTokenFactoryBase.h"

/**
 * Used to ignore parsing anything anything inside a comment block 
 */
class CodeParseTokenFactoryCommentBlock : public CodeParseTokenFactoryBase
{
public:

	//~ Begin CodeParseTokenFactoryBase Interface
	bool IsKeyword(const std::string& keyword) override;
	bool CanCreateToken() const override { return false; }
	std::string GetEndString() const override;
	//~ End CodeParseTokenFactoryBase Interface
};
