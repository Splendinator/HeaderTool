#include "CodeParseTokenFactoryStructMetadata.h"

#include "CodeParseTokenStructMetadata.h"
#include "Editor/ImGuiEditorMacros.h"

bool CodeParseTokenFactoryStructMetadata::IsKeyword(const std::string& keyword)
{
	return keyword == ImGuiEditorMacros::editorStructString;
}

CodeParseTokenBase* CodeParseTokenFactoryStructMetadata::CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory)
{
	return new CodeParseTokenStructMetadata();
}

std::string CodeParseTokenFactoryStructMetadata::GetEndString() const
{
	return ")";
}
