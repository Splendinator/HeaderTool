#pragma once

#include "CodeParseTokenBase.h"

/**
 * Responsible for the individual values inside an enum scope.
 */
class CodeParseTokenEnumValue : public CodeParseTokenBase
{
public:
	CodeParseTokenEnumValue(const std::string& inName, int inValue) : name(inName), value(inValue) {}

	//~ Begin CodeParseTokenBase Interface
	void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface

	std::string name;
	int value;
};
