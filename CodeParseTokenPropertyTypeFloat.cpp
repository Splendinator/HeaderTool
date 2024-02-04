﻿#include "CodeParseTokenPropertyTypeFloat.h"

#include "CodeParseTokenPropertyFloat.h"
#include "DomLog/DomLog.h"

#include <sstream>

bool CodeParseTokenPropertyTypeFloat::IsPropertyType(const std::string& property) const
{
	std::stringstream stream(property);

	std::string dataType; 
	stream >> dataType;

	return dataType == "float";
}

CodeParseTokenBase* CodeParseTokenPropertyTypeFloat::CreateParseTokenType(const std::string& property)
{
	std::stringstream stream(property);

	std::string unused; 
	stream >> unused; // "float"
	stream >> propertyName; // "myFloat"
	
	
	std::string defaultValue = GetDefaultValueString(property);
	if (defaultValue == "")
	{
		defaultValue = "0.0f";
	}
		
	DOMLOG_ERROR_IF(defaultValue.substr(defaultValue.size() - 1, 1) != "f", "default float value not in correct format, must end in f");
	defaultValue = defaultValue.substr(0, defaultValue.size() - 1); // cut off the f
	
	if (defaultValue[defaultValue.size()-1] == '.')
	{
		defaultValue = defaultValue.substr(0, defaultValue.size() - 1); // Cut off the dot
	}
	
	return new CodeParseTokenPropertyFloat(propertyName, std::stof(defaultValue));
}