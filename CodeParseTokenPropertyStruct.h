#pragma once

#include "CodeParseTokenPropertyBase.h"

/** CodeParseTokenPropertyStruct
 *
 * Token representing a Struct property "MyStruct myStruct;"
 * We don't support inline default values like MyStruct myStruct(0, 4.0f).
 */
class CodeParseTokenPropertyStruct : public CodeParseTokenPropertyBase
{
public:

	CodeParseTokenPropertyStruct(const std::string& inPropertyName, const std::string& inStructName) : CodeParseTokenPropertyBase(inPropertyName), structName(inStructName) {}

	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface

	//~ Begin CodeParseTokenPropertyBase Interface
	virtual std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode) const override;
	virtual std::vector<std::string> GetRequiredDependencies() const override { return {structName}; }
	//~ End CodeParseTokenPropertyBase Interface

	std::string structName; // name of the Struct type this pointer points to, i.e "Vec3f", not "position" 
};
