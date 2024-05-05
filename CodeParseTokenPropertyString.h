#pragma once

#include "CodeParseTokenPropertyBase.h"

#include <string>

/** CodeParseTokenPropertyString
*
* This class is used for parsing string properties
*/
class CodeParseTokenPropertyString : public CodeParseTokenPropertyBase
{
public:
	
	CodeParseTokenPropertyString(const std::string& inPropertyName, const std::string& inDefaultValue) : CodeParseTokenPropertyBase(inPropertyName), defaultValue(inDefaultValue) {}

	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface

	//~ Begin CodeParseTokenPropertyBase Interface
	virtual std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const override;
	//~ End CodeParseTokenPropertyBase Interface

private:
	std::string defaultValue;
};
