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

CodeParseTokenBase* CodeParseTokenFactoryStruct::CreateUDTToken(const std::string& udtName, const std::vector<std::string>& baseUdts)
{
	return new CodeParseTokenStruct(udtName, baseUdts);
}
