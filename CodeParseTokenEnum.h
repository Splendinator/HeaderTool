#pragma once

#include "CodeParseTokenBase.h"

/**
 * Token representing an enum. It *doesn't* represent the values, they are handled by CodeParseTokenEnumValue
 */
class CodeParseTokenEnum : public CodeParseTokenBase
{
public:

	CodeParseTokenEnum(const std::string& inName) : name(inName) {}
	
	//~ Begin CodeParseTokenBase Interface
	void WriteToFile(std::ofstream& outputFile) override;
	std::string GetSatisfiedDependency() const { return name; }
	//~ End CodeParseTokenBase Interface

private:

	std::string name; // name of enum "EMyEnum"
};
