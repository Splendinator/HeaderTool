#include "CodeParseTokenFactoryUDT.h"

#include "ImGuiEditorMacros.h"

CodeParseTokenBase* CodeParseTokenFactoryUDT::CreateToken(const std::string& keyword, std::stringstream& stream)
{
	std::string nextString;
	
	stream >> nextString;
	if (nextString != GetUDTKeyword())
	{
		return nullptr;
	}

	std::string udtName;
	stream >> udtName;

	stream >> nextString; 
	DOMLOG_ERROR_IF(nextString != "{", "Expected { after UDT name");

	return CreateUDTToken(udtName);

	// #TODO: Handle base class if one exists
}

std::string CodeParseTokenFactoryUDT::GetEndString() const
{
	return "}";
}
