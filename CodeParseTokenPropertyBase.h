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

	// This should return the generated code to set this property up on an input EditorTypePropertyBase*
	virtual std::string GeneratedCodePropertySetString(const std::string& propertyIndex) const = 0;

	std::string propertyName;
};