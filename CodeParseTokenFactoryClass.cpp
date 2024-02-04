#include "CodeParseTokenFactoryClass.h"

#include "CodeParseTokenClass.h"
#include "ImGuiEditorMacros.h"

bool CodeParseTokenFactoryClass::IsKeyword(const std::string& keyword)
{
	return keyword == ImGuiEditorMacros::editorClassString;
}

CodeParseTokenBase* CodeParseTokenFactoryClass::CreateToken(const std::string& keyword, std::stringstream& stream)
{
	std::string nextString;
	
	stream >> nextString;
	if (nextString != "class")
	{
		return nullptr;
	}

	std::string className;
	stream >> className;

	stream >> nextString; 
	DOMLOG_ERROR_IF(nextString != "{", "Expected { after class name");

	return new CodeParseTokenClass(className);

	// #TODO: Handle base class if one exists
}

std::string CodeParseTokenFactoryClass::GetEndString() const
{
	return "}";
}
