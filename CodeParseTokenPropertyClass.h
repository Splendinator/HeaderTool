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

	//~ Begin CodeParseTokenPropertyFloat Interface
	virtual std::string GeneratedCodePropertySetString(const std::string& propertyTypeCode) const override { return "static_cast<" + className + "*>(static_cast<EditorTypePropertyClass*>(" + propertyTypeCode + ")->GetValue());\n"; }
	//~ End CodeParseTokenPropertyFloat Interface

	std::string className; // name of the class type this pointer points to, i.e "Vec3f", not "position" 
};
