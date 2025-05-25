#include "CodeParseTokenFactoryEnum.h"

#include "CodeParseTokenEnum.h"
#include "Editor/ImGuiEditorMacros.h"

bool CodeParseTokenFactoryEnum::IsKeyword(const std::string& keyword)
{
	return keyword == ImGuiEditorMacros::editorEnumString;
}

CodeParseTokenBase* CodeParseTokenFactoryEnum::CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory)
{
	// enum class EMyEnum
	std::string temp;

	while (temp != "enum")
	{
		stream >> temp;
	}

	std::string enumName;
	stream >> enumName;

	if (enumName == "class")
	{
		stream >> enumName;
	}
	else
	{
		DOMLOG_WARN("Enum", enumName, "isn't an enum class")
	}

	size_t bracketLocation = std::string::npos; 
	while (bracketLocation == std::string::npos)
	{
		stream >> temp;
		bracketLocation = temp.find('{');
	}

	stream.seekg((bracketLocation + 1) - temp.size(), std::ios_base::cur); // Seek back to the '{' in the event of "enum EMyENum {SomeValue,OtherValue}"
	
	return new CodeParseTokenEnum(enumName);
}

std::string CodeParseTokenFactoryEnum::GetEndString() const
{
	return "}";
}

bool CodeParseTokenFactoryEnum::ShouldAddDelimiterAfter() const
{
	return true;
}

int CodeParseTokenFactoryEnum::GetNextValueToUse()
{
	return ++lastUsedValue;
}

void CodeParseTokenFactoryEnum::SetValueJustUsed(int inValue)
{
	lastUsedValue = inValue;
}
