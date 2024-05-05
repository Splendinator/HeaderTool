#pragma once

#include "CodeParseTokenPropertyBase.h"

/** CodeParseTokenPropertyFloat
*
* This class is used to parse float properties
*/
class CodeParseTokenPropertyFloat : public CodeParseTokenPropertyBase
{
public:

	CodeParseTokenPropertyFloat(const std::string& inPropertyName, float inDefaultValue) : CodeParseTokenPropertyBase(inPropertyName), defaultValue(inDefaultValue) {}

	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface

	//~ Begin CodeParseTokenPropertyBase Interface
	virtual std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const override;
	//~ End CodeParseTokenPropertyBase Interface

private:

	float defaultValue;
};
