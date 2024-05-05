#pragma once

#include "CodeParseTokenPropertyBase.h"

/** CodeParseTokenPropertyBool
*
* This class is used for bool properties
*/
class CodeParseTokenPropertyBool : public CodeParseTokenPropertyBase
{
public:
	
	CodeParseTokenPropertyBool(const std::string& inPropertyName, bool inDefaultValue) : CodeParseTokenPropertyBase(inPropertyName), defaultValue(inDefaultValue) {}

	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface

	//~ Begin CodeParseTokenPropertyBase Interface
	virtual std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const override;
	//~ End CodeParseTokenPropertyBase Interface
	
private:
	bool defaultValue = false;
};
