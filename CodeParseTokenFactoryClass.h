#pragma once

#include "CodeParseTokenFactoryUDT.h"

/** CodeParseTokenFactoryClass
*
* This class is used to create CodeParseTokenClass objects
*/
class CodeParseTokenFactoryClass : public CodeParseTokenFactoryUDT
{
public:
	
	//~ Begin CodeParseTokenFactoryBase Interface
	bool IsKeyword(const std::string& keyword) override;
	//~ End CodeParseTokenFactoryBase Interface

	//~ Begin CodeParseTokenFactoryUDT Interface
	virtual std::string GetUDTKeyword() override;
	virtual CodeParseTokenBase* CreateUDTToken(const std::string& udtName) override;
	//~ End CodeParseTokenFactoryUDT Interface

	
	
};
