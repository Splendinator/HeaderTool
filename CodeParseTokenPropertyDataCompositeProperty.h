#pragma once

#include "CodeParseTokenPropertyBase.h"

// Represents a property like "DataCompositeProperty<float> myFloatProperty;"
class CodeParseTokenPropertyDataCompositeProperty : public CodeParseTokenPropertyBase
{
public:

	CodeParseTokenPropertyDataCompositeProperty(const std::string& inPropertyName, const std::string& inDataType) : CodeParseTokenPropertyBase(inPropertyName), dataType(inDataType) {}
	
	//~ Begin CodeParseTokenBase Interface
	void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface

	//~ Begin CodeParseTokenPropertyBase Interface
	std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const override;
	//~ End CodeParseTokenPropertyBase Interface

private:

	std::string dataType; // e.g. "float", "MyClass". Used to only allow selection of properties of this type in the EditorAssetDataComposite.
};
