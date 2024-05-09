#include "CodeParseTokenFactoryProperty.h"

#include "CodeParseTokenPropertyTypeBool.h"
#include "CodeParseTokenPropertyTypeClass.h"
#include "CodeParseTokenPropertyTypeEnum.h"
#include "CodeParseTokenPropertyTypeFloat.h"
#include "CodeParseTokenPropertyTypeInt.h"
#include "CodeParseTokenPropertyTypeString.h"
#include "CodeParseTokenPropertyTypeStruct.h"
#include "CodeParseTokenPropertyTypeVector.h"
#include "ImGuiEditorMacros.h"

class CodeParseTokenPropertyTypeBase;

// CodeParseTokenFactoryProperty
bool CodeParseTokenFactoryProperty::IsKeyword(const std::string& keyword)
{
	return keyword == ImGuiEditorMacros::editorPropertyString;
}

CodeParseTokenBase* CodeParseTokenFactoryProperty::CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory)
{
	// Setup property types -- Order matters here
	CodeParseTokenPropertyTypeFloat propertyTypeFloat;
	CodeParseTokenPropertyTypeInt propertyTypeInt;
	CodeParseTokenPropertyTypeString propertyTypeString;
	CodeParseTokenPropertyTypeBool propertyTypeBool;
	CodeParseTokenPropertyTypeVector propertyTypeVector;
	CodeParseTokenPropertyTypeClass propertyTypeClass;
	CodeParseTokenPropertyTypeEnum propertyTypeEnum;
	CodeParseTokenPropertyTypeStruct propertyTypeStruct;
	
	CodeParseTokenPropertyTypeBase* pPropertyTypes[] = 
	{ 
		&propertyTypeFloat,
		&propertyTypeInt,
		&propertyTypeString,
		&propertyTypeBool,
		&propertyTypeVector,
		&propertyTypeClass,
		&propertyTypeEnum,
		&propertyTypeStruct,
	};

	// Try all property types
	std::string propertyString;
	std::getline(stream, propertyString); // newline after EDITORPROPERTY()
	std::getline(stream, propertyString); // actual property line
	
	for (CodeParseTokenPropertyTypeBase* pPropertyType : pPropertyTypes)
	{
		if (pPropertyType->IsPropertyType(propertyString))
		{
			return pPropertyType->CreateParseTokenType(propertyString);
		}
	}
	
	DOMLOG_ERROR("Unknown property type", propertyString);
	return nullptr;
}
