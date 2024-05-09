#include "CodeParseTokenFactoryCommentBlock.h"

#include "CodeParseTokenScope.h"

bool CodeParseTokenFactoryCommentBlock::IsKeyword(const std::string& keyword)
{
	return keyword == "/*";
}

std::string CodeParseTokenFactoryCommentBlock::GetEndString() const
{
	return "*/";
}

