#include "CodeParseTokenPropertyString.h"

#include "Editor/Types/Properties/WritePropertyToFileUtils.h"

void CodeParseTokenPropertyString::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteStringToFile(outputFile, propertyName, defaultValue);
}

std::string CodeParseTokenPropertyString::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const
{
	return lValueString + " = static_cast<EditorTypePropertyString*>(" + propertyTypeCode + ")->GetValue();\n";
}
