#include "CodeParseTokenPropertyDataCompositeProperty.h"

#include "WritePropertyToFileUtils.h"

void CodeParseTokenPropertyDataCompositeProperty::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteDataCompositePropertyToFile(outputFile, propertyName, dataType);
}

std::string CodeParseTokenPropertyDataCompositeProperty::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const
{
	return lValueString + " = static_cast<EditorTypePropertyDataCompositeProperty*>(" + propertyTypeCode + ")->GetValue();\n";
}
