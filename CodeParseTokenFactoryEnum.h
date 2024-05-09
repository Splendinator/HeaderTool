#pragma once

#include "CodeParseTokenFactoryBase.h"

/**
 * Factory to create CodeParseTokenEnums on EDITORENUM() marked enums
 */
class CodeParseTokenFactoryEnum : public CodeParseTokenFactoryBase
{
public:
	//~ Begin CodeParseTokenFactoryBase Interface
	bool IsKeyword(const std::string& keyword) override;
	CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory) override;
	std::string GetEndString() const override;
	bool ShouldAddDelimiterAfter() const override;
	//~ End CodeParseTokenFactoryBase Interface

	// These are used by "child" CodeParseTokenFactoryEnumValues to determine which enum value to use
	int GetNextValueToUse();
	void SetValueJustUsed(int inValue);

private:

	/* The last used integer value, this is used to figure out the integer value of the next one. It works like this: 
	 *
	 * 	enum class EMyEnum
	 * 	{
	 * 		Red = 3,	<--- This will be 3
	 * 		Green = 2,	<--- This will be 2
	 * 		Blue,		<--- This will be 3 
	 * 	};
	 */
	int lastUsedValue = -1;
};
