#pragma once

#include "CodeParseTokenPropertyBase.h"

enum EVectorDataType
{
	Class,
	Struct,
	Float,
};

/** CodeParseTokenPropertyVector
*
* Token for a vector property "std::vector<float> floatArray;"
*/
class CodeParseTokenPropertyVector : public CodeParseTokenPropertyBase
{
public:
	CodeParseTokenPropertyVector(const std::string& inPropertyName, const std::string& inDataType) : dataType(inDataType), CodeParseTokenPropertyBase(inPropertyName) {}
	
	//~ Begin CodeParseTokenBase Interface
	void WriteToFile(std::ofstream& outputFile) override;
	std::vector<std::string> GetRequiredDependencies() const override { return {dataType}; }; // #TODO: A vector inside a vector fucks this up, we need to figure that out at some point (or we can just enforce structs with vectors in inside vectors)
	//~ End CodeParseTokenBase Interface	

	//~ Begin CodeParseTokenPropertyBase Interface
	std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode) const override;
	//~ End CodeParseTokenPropertyBase Interface

	std::string dataType; // Type of data this vector holds ("float", "MyClass", etc.)
	EVectorDataType dataTypeType; // Type of the data type ("float", "Class", "Enum") etc.
};
