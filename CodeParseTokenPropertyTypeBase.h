#pragma once

#include <string>

class CodeParseTokenBase;

/** CodeParseTokenPropertyTypeBase
*
 * This class just creates the appropriate CodeToken if the input string is correct.
 * i.e "float myFloat = 2.0f;" creates a FloatToken("myFloat",2.0f)
 */
class CodeParseTokenPropertyTypeBase
{
public:
	// Return whether the input data type is this data type
	// Property is the whole line "float x = 2.0f"
	virtual bool IsPropertyType(const std::string& property) const = 0;

	// Create a token for this data type
	virtual CodeParseTokenBase* CreateParseTokenType(const std::string& property) = 0;

protected:
	

	std::string GetDefaultValueString(const std::string& property) const;

	std::string propertyName;
};