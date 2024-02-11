#include "CodeParseTokenFactoryStruct.h"

#include "CodeParseTokenStruct.h"
#include "ImGuiEditorMacros.h"

bool CodeParseTokenFactoryStruct::IsKeyword(const std::string& keyword)
{
	return keyword == ImGuiEditorMacros::editorStructString;
}

std::string CodeParseTokenFactoryStruct::GetUDTKeyword()
{
	return "struct";
}

CodeParseTokenBase* CodeParseTokenFactoryStruct::CreateUDTToken(const std::string& udtName)
{
	return new CodeParseTokenStruct(udtName);
}
