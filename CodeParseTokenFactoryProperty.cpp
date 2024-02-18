#include "CodeParseTokenFactoryProperty.h"

#include "CodeParseTokenPropertyTypeClass.h"
#include "CodeParseTokenPropertyTypeFloat.h"
#include "CodeParseTokenPropertyTypeStruct.h"
#include "CodeParseTokenPropertyTypeVector.h"
#include "ImGuiEditorMacros.h"

class CodeParseTokenPropertyTypeBase;

// CodeParseTokenFactoryProperty
bool CodeParseTokenFactoryProperty::IsKeyword(const std::string& keyword)
{
	return keyword == ImGuiEditorMacros::editorPropertyString;
}

CodeParseTokenBase* CodeParseTokenFactoryProperty::CreateToken(const std::string& keyword, std::stringstream& stream)
{
	// Setup property types
	CodeParseTokenPropertyTypeFloat propertyTypeFloat;
	CodeParseTokenPropertyTypeClass propertyTypeClass;
	CodeParseTokenPropertyTypeStruct propertyTypeStruct;
	CodeParseTokenPropertyTypeVector propertyTypeVector;
	
	CodeParseTokenPropertyTypeBase* pPropertyTypes[] = 
	{ 
		&propertyTypeFloat,
		&propertyTypeClass,
		&propertyTypeStruct,
		&propertyTypeVector,
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
