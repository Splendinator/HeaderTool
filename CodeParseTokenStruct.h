#pragma once

#include "CodeParseTokenBase.h"

/** CodeParseTokenStruct
*
* Token for a Struct
*/
class CodeParseTokenStruct : public CodeParseTokenBase
{
public:

	CodeParseTokenStruct(std::string inStructName) : CodeParseTokenBase(), structName(inStructName) {}
	CodeParseTokenStruct(std::string inStructName, std::string inBaseStruct) : CodeParseTokenBase(), structName(inStructName), baseStruct(inBaseStruct) {}
	
	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface
	
	std::string structName;
	std::string baseStruct;
};