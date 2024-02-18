#pragma once

#include "CodeParseTokenPropertyTypeBase.h"

/** CodeParseTokenPropertyTypeVector
 *
 * Property type factory for vectors (turning "std::vector<float> floatArray;" into a CodeParseTokenPropertyVector) 
 */
class CodeParseTokenPropertyTypeVector : public CodeParseTokenPropertyTypeBase
{
public:

	//~ Begin CodeParseTokenPropertyTypeBase Interface
	bool IsPropertyType(const std::string& property) const override;
	CodeParseTokenBase* CreateParseTokenType(const std::string& property) override;
	//~ End CodeParseTokenPropertyTypeBase Interface
};
