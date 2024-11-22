#pragma once

#include "CodeParseTokenPropertyTypeBase.h"

/** CodeParseTokenPropertyTypeInstancedAsset
 *
 * Property type factory for TInstancedObjectAssetPtr<TClass> 
 */
class CodeParseTokenPropertyTypeInstancedAsset : public CodeParseTokenPropertyTypeBase
{
public:

	//~ Begin CodeParseTokenPropertyTypeBase Interface
	bool IsPropertyType(const std::string& property) const override;
	CodeParseTokenBase* CreateParseTokenType(const std::string& property) override;
	//~ End CodeParseTokenPropertyTypeBase Interface
};
