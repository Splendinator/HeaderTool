#pragma once

#include "CodeParseTokenFactoryBase.h"

/** CodeParseTokenFactoryScope
* 
* This class is responsible for creating CodeParseTokenScope objects.
*/
class CodeParseTokenFactoryScope : public CodeParseTokenFactoryBase
{
public:
	//~ Begin CodeParseTokenFactoryBase Interface
	virtual bool IsKeyword(const std::string& keyword) override;
	virtual bool CanCreateToken() const override { return false; }
	virtual std::string GetEndString() const override;
	//~ End CodeParseTokenFactoryBase Interface
};
