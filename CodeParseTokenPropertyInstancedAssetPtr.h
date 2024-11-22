#pragma once

#include <DomMath/Types.h>

#include "CodeParseTokenPropertyBase.h"

/** CodeParseTokenPropertyInstancedAssetPtr
*
* Token for a TInstacedObjectAssetPtr<TClass> property
*/
class CodeParseTokenPropertyInstancedAssetPtr : public CodeParseTokenPropertyBase
{
public:
	CodeParseTokenPropertyInstancedAssetPtr(const std::string& inPropertyName, const std::string& inClassName) : className(inClassName), CodeParseTokenPropertyBase(inPropertyName) {}
	
	//~ Begin CodeParseTokenBase Interface
	void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface	

	//~ Begin CodeParseTokenPropertyBase Interface
	std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const override;
	//~ End CodeParseTokenPropertyBase Interface

	std::string className; // Class that this pointer points to, child classes will also be selectable in-editor. 
};
