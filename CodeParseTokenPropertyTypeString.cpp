#include "CodeParseTokenPropertyTypeString.h"

#include "CodeParseTokenPropertyString.h"
#include "DomLog/DomLog.h"

#include <sstream>

#include "HeaderToolUtils.h"

bool CodeParseTokenPropertyTypeString::IsPropertyType(const std::string& property) const
{
	std::stringstream stream(property);
	std::string dataType; 
	stream >> dataType;

	for (std::string& stringType : HeaderToolUtils::stringTypes)
	{
		if (stringType == dataType)
		{
			return true;
		}
	}
	return false;
}

CodeParseTokenBase* CodeParseTokenPropertyTypeString::CreateParseTokenType(const std::string& property)
{
	std::stringstream stream(property);
	std::string unused;
	std::string propertyName;

	stream >> unused; // "string"
	stream >> propertyName; // "myString"

	std::string defaultValue = "\"\""; // We keep the default value as \"\" for easier parsing on the other side
	if (propertyName[propertyName.size() - 1] == ';')
	{
		// No default value i.e. "string myString;"
		propertyName = propertyName.substr(0, propertyName.size() - 1); // cut off semicolon
	}
	else
	{
		// Default value i.e. "string myString = \"hello\";"
		defaultValue = GetDefaultValueString(property);
		if (defaultValue.empty())
		{
			defaultValue = "\"\"";
		}
	}
	
	return new CodeParseTokenPropertyString(propertyName, defaultValue);
}
