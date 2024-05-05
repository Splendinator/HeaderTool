#include "CodeParseTokenPropertyTypeBase.h"

#include <sstream>

std::string CodeParseTokenPropertyTypeBase::GetDefaultValueString(const std::string& property) const
{
	size_t assignmentIndex = property.find('=');

	std::string returnString = "";
	
	if (assignmentIndex != std::string::npos)
	{
		size_t semicolonIndex = property.find(';');
		if (semicolonIndex != std::string::npos)
		{
			returnString = property.substr(assignmentIndex + 1, (semicolonIndex - assignmentIndex) - 1);
		}
	}

	// Get rid of leading spaces
	while (std::isspace(returnString.front()))
	{
		returnString = returnString.substr(1, returnString.length());	
	}
	
	return returnString;
}
