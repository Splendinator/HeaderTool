#include "CodeParseTokenPropertyTypeEnum.h"

#include <sstream>
#include "CodeParseTokenPropertyEnum.h"

bool CodeParseTokenPropertyTypeEnum::IsPropertyType(const std::string& property) const
{
	size_t assignmentIndex = property.find('=');
	size_t enumScopeIndex = property.find("::");
	size_t pointerIndex = property.find('*');


	// #JANK: Currently the only way to differentiate enums and structs is that enums have a default type, so we have to force it. "EMyEnum myEnum = EMyEnum::Default"
	
	return
		enumScopeIndex != std::string::npos &&	
		assignmentIndex != std::string::npos &&
		pointerIndex == std::string::npos;
	
}

CodeParseTokenBase* CodeParseTokenPropertyTypeEnum::CreateParseTokenType(const std::string& property)
{
	std::stringstream stream(property);
	std::string temp;
	std::string enumName;
	std::string propertyName;
	std::string defaultValue;
	stream >> enumName;		// "EMyEnum"
	stream >> propertyName;	// "myEnum"
	stream >> temp;			// "="
	stream >> defaultValue; // "EMyEnum::Default"

	size_t semiColonIndex = defaultValue.find(';');
	if (semiColonIndex != std::string::npos)
	{
		defaultValue = defaultValue.substr(0, semiColonIndex);
	}

	size_t size = defaultValue.find("::");
	if (size != std::string::npos)
	{
		// Get rid of the part before "::", we don't need it.
		defaultValue = defaultValue.substr(size+2);
	}
	
	return new CodeParseTokenPropertyEnum(propertyName, enumName, defaultValue);	
}
