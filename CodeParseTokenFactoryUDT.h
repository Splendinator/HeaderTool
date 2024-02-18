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
	CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream) final;
	std::string GetEndString() const final;
	//~ End CodeParseTokenFactoryBase Interface
	
	virtual std::string GetUDTKeyword() = 0;
	virtual CodeParseTokenBase* CreateUDTToken(const std::string& udtName, const std::vector<std::string>& baseUdts) = 0;
};
