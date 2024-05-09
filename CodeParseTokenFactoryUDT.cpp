#include "CodeParseTokenFactoryUDT.h"

#include "CodeParseTokenUDT.h"
#include "ImGuiEditorMacros.h"

CodeParseTokenBase* CodeParseTokenFactoryUDT::CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory)
{
	std::string nextString;
	
	stream >> nextString;
	if (nextString != GetUDTKeyword())
	{
		DOMLOG_ERROR("Expecting", GetUDTKeyword(), "but got", nextString);
		return nullptr;
	}
	
	std::string udtName;
	std::vector<std::string> baseUdts;
	
	stream >> udtName;

	while (nextString != "{")
	{
		stream >> nextString;
		
		if (nextString == "{" || nextString == ":" || nextString == "public" || nextString == "protected" || nextString == "private" || nextString == ",")
		{
			continue;
		}

		if (nextString.find("<") != std::string::npos)
		{
			// We aren't handling templates right now
			continue;
		}


		// if we are here then we're at a class name(s)
		while (true)
		{
			size_t commaPos = nextString.find(",");
			
			std::string baseUdt = nextString.substr(0, commaPos); // if commaPos is string::npos then this will return the whole nextString
			if (baseUdt != "")
			{
				baseUdts.push_back(baseUdt);
			}

			if (commaPos == std::string::npos)
			{
				break;
			}
			
			nextString = nextString.substr(commaPos+1);
		}
	}

	return CreateUDTToken(udtName, baseUdts);
}

std::string CodeParseTokenFactoryUDT::GetEndString() const
{
	return "}";
}
