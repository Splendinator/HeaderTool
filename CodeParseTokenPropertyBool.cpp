#include "CodeParseTokenPropertyBool.h"

#include "Editor/Types/Properties/WritePropertyToFileUtils.h"

void CodeParseTokenPropertyBool::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteBoolToFile(outputFile, propertyName, defaultValue);
}

std::string CodeParseTokenPropertyBool::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const
{
	return lValueString + " = static_cast<EditorTypePropertyBool*>(" + propertyTypeCode + ")->GetValue();\n";
}
