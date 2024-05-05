#pragma once

#include "CodeParseTokenPropertyBase.h"

/** CodeParseTokenPropertyClass
 *
 * Token representing a class property "MyClass* pMyClass = nullptr;"
 */
class CodeParseTokenPropertyClass : public CodeParseTokenPropertyBase
{
public:

	CodeParseTokenPropertyClass(const std::string& inPropertyName, const std::string& inClassName) : CodeParseTokenPropertyBase(inPropertyName), className(inClassName) {}

	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface

	//~ Begin CodeParseTokenPropertyBase Interface
	virtual std::string GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const override;
	//~ End CodeParseTokenPropertyBase Interface

	std::string className; // name of the class type this pointer points to, i.e "Vec3f", not "position" 
};
