#pragma once

#include "CodeParseTokenFactoryBase.h"

/** CodeParseTokenFactoryUDT (User Defined Types)
*
* This is the base factory of structs and classes since they're similar  
*/
class CodeParseTokenFactoryUDT : public CodeParseTokenFactoryBase
{
public:
	
	//~ Begin CodeParseTokenFactoryBase Interface
	CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory) final;
	bool ShouldAddDelimiterAfter() const { return true; }
	std::string GetEndString() const final;
	//~ End CodeParseTokenFactoryBase Interface
	
	virtual CodeParseTokenBase* CreateUDTToken(const std::string& udtName, const std::vector<std::string>& baseUdts) = 0;
};
