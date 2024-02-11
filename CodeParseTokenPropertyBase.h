#pragma once

#include "CodeParseTokenBase.h"

/** CodeParseTokenPropertyBase
*
* Base class used for property tokens.
* When we parse the code base it's changed from raw string data to "tokens" that are a lot more useful
*/
class CodeParseTokenPropertyBase : public CodeParseTokenBase
{
public:
	CodeParseTokenPropertyBase(const std::string& inPropertyName) : CodeParseTokenBase(), propertyName(inPropertyName) {}

	// This should return the generated code to set this property up.
	// propertyTypeCode is the code for the EditorTypePropertyBase*
	// lValueString is what you're assigning to. i.e "pReturnValue->health"
	virtual std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode) const = 0;

	std::string propertyName;
};