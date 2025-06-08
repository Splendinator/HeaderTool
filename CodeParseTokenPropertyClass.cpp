#include "CodeParseTokenPropertyClass.h"

#include "Editor/Types/Properties/WritePropertyToFileUtils.h"

void CodeParseTokenPropertyClass::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteClassToFile(outputFile, propertyName, className);
}

std::string CodeParseTokenPropertyClass::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const
{
	return lValueString + " = static_cast<" + className + "*>(static_cast<EditorTypePropertyClass*>(" + propertyTypeCode + ")->GetValue());\n";
}
