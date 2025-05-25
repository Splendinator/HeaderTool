#include "CodeParseTokenPropertyTypeInstancedAsset.h"

#include "CodeParseTokenPropertyInstancedAssetPtr.h"
#include "DomLog/DomLog.h"

bool CodeParseTokenPropertyTypeInstancedAsset::IsPropertyType(const std::string& property) const
{
	const std::string searchString = "InstancedAssetPtr";
	return property.find(searchString) != std::string::npos;
}

CodeParseTokenBase* CodeParseTokenPropertyTypeInstancedAsset::CreateParseTokenType(const std::string& property)
{
	size_t templateBeginPos = property.find('<');
	size_t templateEndPos = property.find('>');
	size_t semiColonPos = property.find(';');

	DOMLOG_ERROR_IF(property.find('=') != std::string::npos, "We don't support assignment to vectors")
	
	std::string classType = property.substr(templateBeginPos + 1, templateEndPos - templateBeginPos - 1);
	std::string propertyName = property.substr(templateEndPos + 2, semiColonPos - templateEndPos - 2);
	
	return new CodeParseTokenPropertyInstancedAssetPtr(propertyName, classType);
}
