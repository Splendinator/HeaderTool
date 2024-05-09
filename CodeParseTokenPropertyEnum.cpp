#include "CodeParseTokenPropertyEnum.h"

void CodeParseTokenPropertyEnum::WriteToFile(std::ofstream& outputFile)
{
	outputFile << "enum " << enumName << " " << propertyName << " " << defaultValue << std::endl;
}

std::vector<std::string> CodeParseTokenPropertyEnum::GetRequiredDependencies() const
{
	return {enumName};
}

std::string CodeParseTokenPropertyEnum::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const
{
	return lValueString + " = static_cast<" + enumName + ">(static_cast<EditorTypePropertyEnum*>(" + propertyTypeCode + ")->GetValue());\n";
}
