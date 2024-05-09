#include "CodeParseTokenFactoryEnumValue.h"

#include "CodeParseTokenEnumValue.h"
#include "CodeParseTokenFactoryEnum.h"

bool CodeParseTokenFactoryEnumValue::IsKeyword(const std::string& keyword)
{
	// This function is more eliminative.
	// The reason we can get away with it is that this factory only turns on while we're inside and enum's scope so it won't pick up anything we don't want it to.
	
	if (keyword.find(',') != std::string::npos)
	{
		return false;
	}

	if (keyword.find('=') != std::string::npos)
	{
		return false;
	}

	if (keyword.find('}') != std::string::npos)
	{
		return false;
	}

	return true;
}

CodeParseTokenBase* CodeParseTokenFactoryEnumValue::CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory)
{
	CodeParseTokenFactoryEnum* pEnumFactory = static_cast<CodeParseTokenFactoryEnum*>(pOuterScopedFactory);
	
	std::string temp;
	int value = 0;
	
	const size_t preValuePos = stream.tellg();
	stream >> temp;

	if (temp != "=")
	{
		// if there is no explicit asignment of value, just use the next one.
		value = pEnumFactory->GetNextValueToUse();
		
		// we also need to seek back in-case value is "};" at the end of the enum, as this will mess up the scope of the "parent" enum factory.
		stream.seekg(preValuePos);
	}
	else
	{
		stream >> temp;
		value = std::stoi(temp);

		pEnumFactory->SetValueJustUsed(value);
	}
	
	return new CodeParseTokenEnumValue(keyword, value);
}
