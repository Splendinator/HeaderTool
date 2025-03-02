#include "CodeParseTokenPropertyTypeClass.h"

#include "CodeParseTokenPropertyClass.h"

#include <sstream>

bool CodeParseTokenPropertyTypeClass::IsPropertyType(const std::string& property) const
{
	size_t pointerIndex = property.find('*');
	size_t assignmentIndex = property.find('=');

	return pointerIndex != std::string::npos
		&& (assignmentIndex == std::string::npos || pointerIndex < assignmentIndex); // Stars after the equals could be "int x = 2*8" so cut them out
	
}

CodeParseTokenBase* CodeParseTokenPropertyTypeClass::CreateParseTokenType(const std::string& property)
{
	std::stringstream stream(property);
	std::string className;
	std::string classPropertyName;
	stream >> className;

	if (className == "const")
	{
		// Ignore const
		stream >> className;
	}
	stream >> classPropertyName;
	
	if (className[className.length()-1] == '*')
	{
		className = className.substr(0, className.length()-1);
	}
	
	if (classPropertyName[classPropertyName.length()-1] == ';')
	{
		classPropertyName = classPropertyName.substr(0, classPropertyName.length()-1);
	}
	
	return new CodeParseTokenPropertyClass(classPropertyName, className);
}
