#include "CodeParseTokenPropertyTypeVector.h"

#include "CodeParseTokenPropertyVector.h"
#include "HeaderToolUtils.h"

#include "DomLog/DomLog.h"

bool CodeParseTokenPropertyTypeVector::IsPropertyType(const std::string& property) const
{
	const std::string searchString = "std::vector";
	return property.find(searchString) != std::string::npos;
}

CodeParseTokenBase* CodeParseTokenPropertyTypeVector::CreateParseTokenType(const std::string& property)
{
	size_t templateBeginPos = property.find('<');
	size_t templateEndPos = property.find('>');
	size_t semiColonPos = property.find(';');

	DOMLOG_ERROR_IF(property.find('=') != std::string::npos, "We don't support assignment to vectors")
	
	std::string dataType = property.substr(templateBeginPos + 1, templateEndPos - templateBeginPos - 1);
	std::string propertyName = property.substr(templateEndPos + 2, semiColonPos - templateEndPos - 2);

	size_t pointerIndex = dataType.find('*');
	if (pointerIndex != std::string::npos)
	{
		dataType = dataType.substr(0, pointerIndex);
	}

	size_t stdIndex = dataType.find("std::");
	if (stdIndex != std::string::npos)
	{
		dataType = dataType.substr(stdIndex + std::size("std::") - 1);
	}
	
	return new CodeParseTokenPropertyVector(propertyName, dataType);
}
