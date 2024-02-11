#pragma once

#include "CodeParseTokenPropertyTypeBase.h"

/** CodeParseTokenPropertyTypeStruct
*
* This struct is used for struct properties (e.g "MyStruct* myStruct = nullptr;")
*/
struct CodeParseTokenPropertyTypeStruct : public CodeParseTokenPropertyTypeBase 
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
