#include "CodeParseTokenFactoryClass.h"

#include "CodeParseTokenClass.h"
#include "ImGuiEditorMacros.h"

bool CodeParseTokenFactoryClass::IsKeyword(const std::string& keyword)
{
	return keyword == ImGuiEditorMacros::editorClassString;
}

std::string CodeParseTokenFactoryClass::GetUDTKeyword()
{
	return "class";
}

CodeParseTokenBase* CodeParseTokenFactoryClass::CreateUDTToken(const std::string& udtName, const std::vector<std::string>& baseUdts)
{
	return new CodeParseTokenClass(udtName, baseUdts);
}
