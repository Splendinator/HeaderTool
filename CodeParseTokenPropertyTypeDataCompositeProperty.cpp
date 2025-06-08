#include "CodeParseTokenPropertyTypeDataCompositeProperty.h"

#include "CodeParseTokenPropertyDataCompositeProperty.h"
#include "DomLog/DomLog.h"
#include "DomUtils/DomUtils.h"

bool CodeParseTokenPropertyTypeDataCompositeProperty::IsPropertyType(const std::string& property) const
{
	const std::string searchString = "DataCompositeProperty<";
	std::string trimmedProperty = dmut::TrimWhitespace(property);
	const std::string actualString = trimmedProperty.substr(0, searchString.length());
	return actualString == searchString;
}

CodeParseTokenBase* CodeParseTokenPropertyTypeDataCompositeProperty::CreateParseTokenType(const std::string& property)
{
	// "DataCompositeProperty<float> myFloat = 2.0f;"

	size_t typeStartPos = property.find('<');
	if (typeStartPos == std::string::npos)
	{
		DOMLOG_ERROR("Expecting '<'", property);
		return nullptr;
	}
	
	size_t typeEndPos = property.find('>');
	if (typeEndPos == std::string::npos)
	{
		DOMLOG_ERROR("Expecting '>'", property);
		return nullptr;
	}

	size_t nameEndPos = property.find('=', typeEndPos);
	if (nameEndPos == std::string::npos)
	{
		nameEndPos = property.find(';', typeEndPos);	
	}
	if (nameEndPos == std::string::npos)
	{
		DOMLOG_ERROR("Expecting ';' or '='", property);
		return nullptr;
	}
	
	std::string typeName = property.substr(typeStartPos + 1, typeEndPos - typeStartPos - 1);
	if (typeName.back() == '*')
	{
		// We don't care if it's a pointer, we just need the string name of the type.
		typeName.pop_back();
	}

	std::string propertyName = property.substr(typeEndPos + 1, nameEndPos - typeEndPos - 1);
	dmut::TrimWhitespaceInline(propertyName);

	return new CodeParseTokenPropertyDataCompositeProperty(propertyName, typeName);
}
