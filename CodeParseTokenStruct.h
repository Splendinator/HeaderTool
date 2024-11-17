#pragma once

#include "CodeParseTokenUDT.h"

/** CodeParseTokenStruct
*
* Token for a Struct
*/
class CodeParseTokenStruct : public CodeParseTokenUDT
{
public:

	CodeParseTokenStruct(std::string inName, std::vector<std::string> baseNames = {}) : CodeParseTokenUDT(inName, baseNames) {}
	
	//~ Begin CodeParseTokenUDT Interface
	virtual std::string GetUDTTypeName() override {return "Struct";}
	virtual ECodeParseTokenPriority GetPriorityWithinBlock() const override {return ECodeParseTokenPriority::High;}
	//~ End CodeParseTokenUDT Interface
	
};