#include "CodeParseTokenFactoryClassMetadata.h"

#include "CodeParseTokenClassMetadata.h"
#include "ImGuiEditorMacros.h"

bool CodeParseTokenFactoryClassMetadata::IsKeyword(const std::string& keyword)
{
	return keyword == ImGuiEditorMacros::editorClassString;
}

CodeParseTokenBase* CodeParseTokenFactoryClassMetadata::CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory)
{
	return new CodeParseTokenClassMetadata();
}

std::string CodeParseTokenFactoryClassMetadata::GetEndString() const
{
	return ")";
}
