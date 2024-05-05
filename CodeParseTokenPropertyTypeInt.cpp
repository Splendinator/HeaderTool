#include "CodeParseTokenPropertyTypeInt.h"

#include "CodeParseTokenPropertyInt.h"
#include "DomLog/DomLog.h"

#include <sstream>

#include "HeaderToolUtils.h"

bool CodeParseTokenPropertyTypeInt::IsPropertyType(const std::string& property) const
{
	std::stringstream stream(property);
	std::string dataType; 
	stream >> dataType;

	for (std::string& intType : HeaderToolUtils::intTypes)
	{
		if (intType == dataType)
		{
			return true;
		}
	}
	return false;
}

CodeParseTokenBase* CodeParseTokenPropertyTypeInt::CreateParseTokenType(const std::string& property)
{
	std::stringstream stream(property);
	std::string unused;
	std::string propertyName;

	stream >> unused; // "int"
	stream >> propertyName; // "myInt"

	std::string defaultValue = "0";
	if (propertyName[propertyName.size() - 1] == ';')
	{
		// No default value i.e. "int myInt;"
		propertyName = propertyName.substr(0, propertyName.size() - 1); // cut off semicolon
	}
	else
	{
		// Default value i.e. "int myInt = 10;"
		defaultValue = GetDefaultValueString(property);
		if (defaultValue.empty())
		{
			defaultValue = "0";
		}
	}

	return new CodeParseTokenPropertyInt(propertyName, std::stoi(defaultValue));
}
