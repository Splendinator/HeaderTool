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
	virtual CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream) override;
	virtual std::string GetEndString() const override;
	//~ End CodeParseTokenFactoryBase Interface
};
