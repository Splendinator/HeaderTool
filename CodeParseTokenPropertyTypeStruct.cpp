#include "CodeParseTokenPropertyTypeStruct.h"

#include "CodeParseTokenPropertyStruct.h"

#include <sstream>

bool CodeParseTokenPropertyTypeStruct::IsPropertyType(const std::string& property) const
{
	// #OPTIMISE: Can be optimised to just go through string once
	size_t pointerIndex = property.find('*');
	size_t assignmentIndex = property.find('=');
	size_t bracketIndex = property.find('(');
	size_t curlyIndex = property.find('{');
	size_t templateIndex = property.find('<');
	
	return
		pointerIndex == std::string::npos &&	// We only allow inline structs, not pointers to.
		assignmentIndex == std::string::npos && // We don't allow asignments
		bracketIndex == std::string::npos &&	// We don't allow per-usage default values (i.e constructor calls)
		curlyIndex == std::string::npos &&
		templateIndex == std::string::npos;		// Eliminate templated things like std::vector<>
	
}

CodeParseTokenBase* CodeParseTokenPropertyTypeStruct::CreateParseTokenType(const std::string& property)
{
	std::stringstream stream(property);
	std::string structName;
	std::string propertyName;
	stream >> structName;
	stream >> propertyName;
	
	if (propertyName[propertyName.length()-1] == ';')
	{
		propertyName = propertyName.substr(0, propertyName.length()-1);
	}
	
	return new CodeParseTokenPropertyStruct(propertyName, structName);
}
