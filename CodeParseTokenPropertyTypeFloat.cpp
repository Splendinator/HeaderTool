#include "CodeParseTokenPropertyTypeFloat.h"

#include "CodeParseTokenPropertyFloat.h"
#include "DomLog/DomLog.h"

#include <sstream>

#include "HeaderToolUtils.h"

bool CodeParseTokenPropertyTypeFloat::IsPropertyType(const std::string& property) const
{
	std::stringstream stream(property);

	std::string dataType; 
	stream >> dataType;

	for (std::string& floatType : HeaderToolUtils::floatTypes)
	{
		if (floatType == dataType)
		{
			return true;
		}
	}
	return false;
}

CodeParseTokenBase* CodeParseTokenPropertyTypeFloat::CreateParseTokenType(const std::string& property)
{
	std::stringstream stream(property);

	std::string unused;
	std::string propertyName;
	
	stream >> unused; // "float"
	stream >> propertyName; // "myFloat"
	
	
	std::string defaultValue = "0.0f";
	if (propertyName[propertyName.size() - 1] == ';')
	{
		// No default value i.e. "float myFloat;"
		propertyName = propertyName.substr(0, propertyName.size() - 1); // cut off semicolon
	}
	else
	{
		// Default value i.e. "float myFloat = 2.0f;"
		defaultValue = GetDefaultValueString(property);
		if (defaultValue == "")
		{
			defaultValue = "0.0f";
		}
	}
	
	if (defaultValue.find('.') == std::string::npos)
	{
		// In case of float f = 5; treat it as 5.0f;
		defaultValue.append(".0f");
	}
	
	DOMLOG_ERROR_IF(defaultValue.substr(defaultValue.size() - 1, 1) != "f", "default float value not in correct format, must end in f");
	defaultValue = defaultValue.substr(0, defaultValue.size() - 1); // cut off the f
	
	if (defaultValue[defaultValue.size()-1] == '.')
	{
		defaultValue = defaultValue.substr(0, defaultValue.size() - 1); // Cut off the dot
	}
	
	return new CodeParseTokenPropertyFloat(propertyName, std::stof(defaultValue));
}
