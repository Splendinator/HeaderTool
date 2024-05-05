#pragma once

#include "CodeParseTokenPropertyTypeBase.h"

/** CodeParseTokenPropertyTypeBool
 *
 * Handles creating bool property tokens
 */
class CodeParseTokenPropertyTypeBool : public CodeParseTokenPropertyTypeBase
{
public:
	//~ Begin CodeParseTokenPropertyTypeBase Interface
	bool IsPropertyType(const std::string& property) const override;
	//~ End CodeParseTokenPropertyTypeBase Interface

	protected:
	//~ Begin CodeParseTokenPropertyTypeBase Interface
	CodeParseTokenBase* CreateParseTokenType(const std::string& property) override;
	//~ End CodeParseTokenPropertyTypeBase Interface
};
