#include "CodeParseTokenPropertyTypeBool.h"

#include "CodeParseTokenPropertyBool.h"
#include "DomLog/DomLog.h"

#include <sstream>

#include "HeaderToolUtils.h"

bool CodeParseTokenPropertyTypeBool::IsPropertyType(const std::string& property) const
{
	std::stringstream stream(property);
	std::string dataType; 
	stream >> dataType;

	for (std::string& boolType : HeaderToolUtils::boolTypes)
	{
		if (boolType == dataType)
		{
			return true;
		}
	}
	return false;
}

CodeParseTokenBase* CodeParseTokenPropertyTypeBool::CreateParseTokenType(const std::string& property)
{
	std::stringstream stream(property);
	std::string unused;
	std::string propertyName;

	stream >> unused; // "bool"
	stream >> propertyName; // "bBool"

	std::string defaultValue = "false";
	if (propertyName[propertyName.size() - 1] == ';')
	{
		// No default value i.e. "bool bBool;"
		propertyName = propertyName.substr(0, propertyName.size() - 1); // cut off semicolon
	}
	else
	{
		// Default value i.e. "bool bBool = true;"
		defaultValue = GetDefaultValueString(property);
		if (defaultValue.empty())
		{
			defaultValue = "false";
		}
		else if (defaultValue != "true" && defaultValue != "false")
		{
			DOMLOG_ERROR("We only support boolean default values of 'true' or 'false'")
			defaultValue = "false";
		}
	}
	
	bool boolValue = (defaultValue == "true");

	return new CodeParseTokenPropertyBool(propertyName, boolValue);
}
