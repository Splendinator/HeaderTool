#pragma once

#include "CodeParseTokenPropertyBase.h"

class CodeParseTokenPropertyEnum : public CodeParseTokenPropertyBase
{
public:
	CodeParseTokenPropertyEnum(const std::string& inPropertyName, const std::string& inEnumName, const std::string& inDefaultValue)
		: CodeParseTokenPropertyBase(inPropertyName), enumName(inEnumName), defaultValue(inDefaultValue) {}

	//~ Begin CodeParseTokenPropertyBase Interface
	void WriteToFile(std::ofstream& outputFile) override;
	std::vector<std::string> GetRequiredDependencies() const override;
	std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const override;
	//~ End CodeParseTokenPropertyBase Interface

	std::string enumName;
	std::string defaultValue;
};
