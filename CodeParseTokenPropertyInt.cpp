#include "CodeParseTokenPropertyInt.h"

#include "Editor/Types/Properties/WritePropertyToFileUtils.h"

void CodeParseTokenPropertyInt::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteIntToFile(outputFile, propertyName, defaultValue);
}

std::string CodeParseTokenPropertyInt::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const
{
	return lValueString + " = static_cast<EditorTypePropertyInt*>(" + propertyTypeCode + ")->GetValue();\n";
}
