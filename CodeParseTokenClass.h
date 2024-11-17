#pragma once

#include "CodeParseTokenUDT.h"

/** CodeParseTokenClass
*
* Token for a class
*/
class CodeParseTokenClass : public CodeParseTokenUDT
{
public:
	CodeParseTokenClass(std::string inName, std::vector<std::string> baseNames = {}) : CodeParseTokenUDT(inName, baseNames) {}
	
	//~ Begin CodeParseTokenUDT Interface
	virtual std::string GetUDTTypeName() override {return "Class";}
	virtual ECodeParseTokenPriority GetPriorityWithinBlock() const override {return ECodeParseTokenPriority::High;}
	//~ End CodeParseTokenUDT Interface
};