#include "CodeParseTokenPropertyTypeBase.h"

#include <sstream>

std::string CodeParseTokenPropertyTypeBase::GetDefaultValueString(const std::string& property) const
{
	size_t assignmentIndex = property.find('=');

	if (assignmentIndex != std::string::npos)
	{
		size_t semicolonIndex = property.find(';');
		if (semicolonIndex != std::string::npos)
		{
			return property.substr(assignmentIndex + 1, (semicolonIndex - assignmentIndex) - 1);
		}
	}
	
	return "";
}
