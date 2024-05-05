#pragma once

#include "CodeParseTokenPropertyBase.h"

/** CodeParseTokenPropertyInt
*
* This class is used for parsing int properties
*/
class CodeParseTokenPropertyInt : public CodeParseTokenPropertyBase
{
public:
	
	CodeParseTokenPropertyInt(const std::string& inPropertyName, int inDefaultValue) : CodeParseTokenPropertyBase(inPropertyName), defaultValue(inDefaultValue) {}

	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface

	//~ Begin CodeParseTokenPropertyBase Interface
	virtual std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const override;
	//~ End CodeParseTokenPropertyBase Interface

private:
	int defaultValue;
};
